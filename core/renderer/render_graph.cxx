// <core/resources/render_graph.cxx> -*- C++ -*-

//  Sapling 3D Game Engine
//  Copyright (C) 2026  Tyler Swann, Georgia Kannelis
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License v2.1 as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//  USA

#include "render_graph.h"

namespace sap::core::renderer {

void RenderGraph::add_resource(
    const std::string& name, vk::Format format, vk::Extent2D extent,
    vk::ImageUsageFlags usage, vk::ImageLayout initial_layout,
    vk::ImageLayout final_layout
) {
    // Use auto and designated initialisers
    Resource resource{};
    resource.name = name;                     // Store human-readable identifier
    resource.format = format;                 // Define pixel format and bit depth
    resource.extent = extent;                 // Set resource dimensions
    resource.usage = usage;                   // Specify intended usage patterns
    resource.initial_layout = initial_layout; // Define starting layout state
    resource.final_layout = final_layout;     // Define required ending state

    m_resources[name] = std::move(resource); // Register in the global resource map
}

// Pass registration interface for defining rendering operations and their dependencies
// This method establishes the logical structure of rendering without immediate execution
void RenderGraph::add_pass(
    const std::string& name, const std::vector<std::string>& inputs,
    const std::vector<std::string>& outputs,
    std::function<void(vk::raii::CommandBuffer&)> execute_func
) {
    // Use auto and designated initialisers
    Pass pass{};
    pass.name = name;                 // Assign descriptive identifier
    pass.inputs = inputs;             // List all resources this pass reads
    pass.outputs = outputs;           // List all resources this pass writes
    pass.execute_func = execute_func; // Store the actual rendering implementation

    m_passes.push_back(pass); // Add to the ordered pass list
}

RenderGraph::Resource *RenderGraph::get_resource(const std::string& name) {
    auto it = m_resources.find(name);
    return (it != m_resources.end()) ? &it->second : nullptr;
}

void RenderGraph::compile() {
    // Dependency Graph Construction

    // Build bidirectional dependency relationships between m_passes
    std::vector<std::vector<size_t>> dependencies(
        m_passes.size()
    ); //< What each pass depends on

    std::vector<std::vector<size_t>> dependents(
        m_passes.size()
    ); //< What depends on each pass

    // Track which pass produces each resource (write-after-write dependencies)
    std::unordered_map<std::string, size_t> resource_writers;

    // Dependency Discovery Through Resource Usage Analysis
    // Analyze each pass to determine data flow relationships
    for (size_t i = 0; i < m_passes.size(); ++i) {
        const auto& pass = m_passes[i];

        // Process input dependencies - this pass must wait for producers
        for (const auto& input : pass.inputs) {
            auto it = resource_writers.find(input);

            if (it != resource_writers.end()) {
                //! Use ::at() or bounds check
                // Found the pass that produces this input - create dependency link
                dependencies[i].push_back(
                    it->second
                );                                   //< This pass depends on the producer
                dependents[it->second].push_back(i); //< Producer has this as dependent
            }
        }

        // Register output production - subsequent m_passes may depend on these
        for (const auto& output : pass.outputs) {
            resource_writers[output] = i; // Record this pass as producer
        }
    }

    // Topological Sort for Optimal Execution Order

    // Use depth-first search to compute valid execution sequence while detecting cycles
    std::vector<bool> visited(m_passes.size(), false);  // Track completed nodes
    std::vector<bool> in_stack(m_passes.size(), false); // Track current recursion path

    std::function<void(size_t)> visit = [&](size_t node) {
        if (in_stack[node]) {
            // Cycle detection - circular dependency found
            throw std::runtime_error("Cycle detected in rendergraph");
        }

        if (visited[node]) {
            return; // Already processed this node and its dependencies
        }

        in_stack[node] = true; // Mark as currently being processed

        // Recursively process all dependent m_passes first (post-order traversal)
        for (auto dependent : dependents[node]) {
            visit(dependent);
        }

        in_stack[node] = false;            // Remove from current path
        visited[node] = true;              // Mark as completely processed
        m_execution_order.push_back(node); // Add to execution sequence
    };

    // Process all unvisited nodes to handle disconnected graph components
    for (size_t i = 0; i < m_passes.size(); ++i) {
        if (!visited[i]) {
            visit(i);
        }
    }

    // Automatic Synchronization Object Creation

    // Generate semaphores for all dependencies identified during analysis
    for (size_t i = 0; i < m_passes.size(); ++i) {
        for (auto dep : dependencies[i]) {
            // Create a GPU semaphore for this dependency relationship
            // The dependent pass will wait on this semaphore before executing
            m_semaphores.emplace_back(m_device.createSemaphore({}));
            m_semaphore_signal_wait_pairs.emplace_back(
                dep, i
            ); // (producer, consumer) pair
        }
    }

    // Physical Resource Allocation and Creation

    // Transform resource descriptions into actual GPU objects
    for (auto& [name, resource] : m_resources) {
        // Configure image creation parameters based on resource description
        vk::ImageCreateInfo image_info;
        image_info
            .setImageType(vk::ImageType::e2D) // 2D texture/render target
            .setFormat(resource.format)       // Pixel format from description
            .setExtent({resource.extent.width, resource.extent.height, 1}) // Dimensions
            .setMipLevels(1)                         // Single mip level for simplicity
            .setArrayLayers(1)                       // Single layer (not array texture)
            .setSamples(vk::SampleCountFlagBits::e1) // No multisampling
            .setTiling(vk::ImageTiling::eOptimal)    // GPU-optimal memory layout
            .setUsage(resource.usage)                // Usage flags from registration
            .setSharingMode(vk::SharingMode::eExclusive) // Single queue family access
            .setInitialLayout(
                vk::ImageLayout::eUndefined
            ); // Initial layout (will be transitioned)

        resource.image = m_device.createImage(image_info); // Create the GPU image object

        // Allocate backing memory for the image
        vk::MemoryRequirements mem_reqs = resource.image.getMemoryRequirements();

        vk::MemoryAllocateInfo alloc_info;
        alloc_info
            .setAllocationSize(mem_reqs.size) // Required memory size
            .setMemoryTypeIndex(_M_find_memory_type(
                mem_reqs.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            )); // GPU-local memory

        resource.memory = m_device.allocateMemory(alloc_info); // Allocate GPU memory
        resource.image.bindMemory(*resource.memory, 0);        // Bind memory to image

        // Create image view for shader access
        vk::ImageViewCreateInfo view_info;
        view_info
            .setImage(*resource.image)           // Reference the created image
            .setViewType(vk::ImageViewType::e2D) // 2D view type
            .setFormat(resource.format)          // Match image format
            .setSubresourceRange(
                {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
            ); // Full image access

        resource.view =
            m_device.createImageView(view_info); // Create shader-accessible view
    }
}
void RenderGraph::execute(vk::raii::CommandBuffer& cmdbuf, vk::Queue queue) {
    // Execution state management for dynamic synchronization
    std::vector<vk::CommandBuffer> cmdbufs;     //< Command buffer storage
    std::vector<vk::Semaphore> wait_semaphores; //< Sync dependencies for current pass
    std::vector<vk::PipelineStageFlags> wait_stages; // Pipeline stages to wait on
    std::vector<vk::Semaphore>
        signal_semaphores; //< Semaphores to signal after current pass

    // Ordered Pass Execution with Automatic Dependency Management
    // Execute each pass in the computed dependency-safe order
    for (auto pidx : m_execution_order) {
        const auto& pass = m_passes[pidx];

        // Synchronization Setup - Collect Dependencies for Current Pass
        // Determine what this pass must wait for before executing
        wait_semaphores.clear();
        wait_stages.clear();

        for (size_t i = 0; i < m_semaphore_signal_wait_pairs.size(); ++i) {
            if (m_semaphore_signal_wait_pairs[i].second == pidx) {
                // This pass depends on the completion of another pass
                wait_semaphores.push_back(
                    *m_semaphores[i]
                ); // Wait for dependency completion
                wait_stages.push_back(
                    vk::PipelineStageFlagBits::eColorAttachmentOutput
                ); // Wait at output stage
            }
        }

        // Collect semaphores that this pass will signal for dependent m_passes
        signal_semaphores.clear();
        for (size_t i = 0; i < m_semaphore_signal_wait_pairs.size(); ++i) {
            if (m_semaphore_signal_wait_pairs[i].first == pidx) {
                // Other m_passes depend on this pass's completion
                signal_semaphores.push_back(
                    *m_semaphores[i]
                ); // Signal completion for dependents
            }
        }

        // Command Buffer Preparation and Resource Layout Transitions
        // Set up command recording and transition resources to appropriate layouts
        cmdbuf.begin({}); // Begin command recording

        // Transition input resources to shader-readable layouts
        for (const auto& input : pass.inputs) {
            auto& resource = m_resources[input];

            vk::ImageMemoryBarrier barrier;
            barrier
                .setOldLayout(resource.initial_layout) // Current resource layout
                .setNewLayout(
                    vk::ImageLayout::eShaderReadOnlyOptimal
                ) // Target layout for reading
                .setSrcQueueFamilyIndex(
                    VK_QUEUE_FAMILY_IGNORED
                ) // No queue family transfer
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setImage(*resource.image) // Target image
                .setSubresourceRange(
                    {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
                ) // Full image range
                .setSrcAccessMask(
                    vk::AccessFlagBits::eMemoryWrite
                ) // Previous write access
                .setDstAccessMask(
                    vk::AccessFlagBits::eShaderRead
                ); // Required read access

            // Insert pipeline barrier for safe layout transition
            cmdbuf.pipelineBarrier(
                vk::PipelineStageFlagBits::eAllCommands,    // Wait for all previous work
                vk::PipelineStageFlagBits::eFragmentShader, // Enable fragment shader
                                                            // access
                vk::DependencyFlagBits::eByRegion,          // Region-local dependency
                nullptr, nullptr, barrier                   // Image barrier only
            );
        }

        // Transition output resources to render target layouts
        for (const auto& output : pass.outputs) {
            auto& resource = m_resources[output];

            vk::ImageMemoryBarrier barrier;
            barrier
                .setOldLayout(resource.initial_layout) // Current layout state
                .setNewLayout(
                    vk::ImageLayout::eColorAttachmentOptimal
                ) // Optimal for color output
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setImage(*resource.image)
                .setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1})
                .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead) // Previous read access
                .setDstAccessMask(
                    vk::AccessFlagBits::eColorAttachmentWrite
                ); // Required write access

            // Insert barrier for safe transition to writable state
            cmdbuf.pipelineBarrier(
                vk::PipelineStageFlagBits::eAllCommands,
                vk::PipelineStageFlagBits::eColorAttachmentOutput, // Enable color
                                                                   // attachment writes
                vk::DependencyFlagBits::eByRegion, nullptr, nullptr, barrier
            );
        }

        // Pass Execution - Execute the Actual Rendering Logic
        // Call the user-provided rendering function with prepared command buffer
        pass.execute_func(cmdbuf); // Execute pass-specific rendering

        // Final Layout Transitions - Prepare Resources for Subsequent Use
        // Transition output resources to their final required layouts
        for (const auto& output : pass.outputs) {
            auto& resource = m_resources[output];

            vk::ImageMemoryBarrier barrier;
            barrier
                .setOldLayout(
                    vk::ImageLayout::eColorAttachmentOptimal
                )                                    // Current writable layout
                .setNewLayout(resource.final_layout) // Required final layout
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setImage(*resource.image)
                .setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1})
                .setSrcAccessMask(
                    vk::AccessFlagBits::eColorAttachmentWrite
                ) // Previous write operations
                .setDstAccessMask(
                    vk::AccessFlagBits::eMemoryRead
                ); // Enable subsequent reads

            // Insert final barrier for layout transition
            cmdbuf.pipelineBarrier(
                vk::PipelineStageFlagBits::eColorAttachmentOutput, // After color writes
                                                                   // complete
                vk::PipelineStageFlagBits::eAllCommands, // Before any subsequent work
                vk::DependencyFlagBits::eByRegion, nullptr, nullptr, barrier
            );
        }

        // Command Submission with Synchronization
        // Submit command buffer with appropriate dependency and signaling semaphores
        cmdbuf.end(); // Finalize command recording

        vk::SubmitInfo submit_info;
        submit_info
            .setWaitSemaphoreCount(
                static_cast<uint32_t>(wait_semaphores.size())
            )                                           // Dependencies to wait for
            .setPWaitSemaphores(wait_semaphores.data()) // Dependency semaphores
            .setPWaitDstStageMask(wait_stages.data())   // Pipeline stages to wait at
            .setCommandBufferCount(1)                   // Single command buffer
            .setPCommandBuffers(&*cmdbuf)               // Command buffer to execute
            .setSignalSemaphoreCount(
                static_cast<uint32_t>(signal_semaphores.size())
            )                                                // Semaphores to signal
            .setPSignalSemaphores(signal_semaphores.data()); // Signal semaphores

        queue.submit(submit_info, nullptr); // Submit to GPU queue
    }
}

auto RenderGraph::_M_find_memory_type(uint32_t type_filter, vk::MemoryPropertyFlags props)
    -> uint32_t {
    // Implementation to find suitable memory type

    return 0;
}

} // namespace sap::core::renderer
