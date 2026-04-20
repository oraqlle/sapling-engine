// <core/resources/renderer_pipeline.cxx> -*- C++ -*-

//  Sapling 3D Game Engine
//  Copyright (C) 2026  Tyler Swann, Georgia Kanellis
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

#include "renderer_pipeline.h"

namespace sap::core::renderer {

auto transition_image_layout(
    vk::raii::CommandBuffer& cmdbuf, vk::Image image, vk::Format format,
    vk::ImageLayout old_layout, vk::ImageLayout new_layout
) -> void {

    // Configure the basic image memory barrier structure
    // This barrier will coordinate memory access and layout transitions
    vk::ImageMemoryBarrier barrier;
    barrier
        .setOldLayout(old_layout) // Current image layout state
        .setNewLayout(new_layout) // Target layout after transition
        .setSrcQueueFamilyIndex(
            VK_QUEUE_FAMILY_IGNORED
        ) // No queue family ownership transfer
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED) // Same queue family throughout
        .setImage(image) // Target image for the transition
        .setSubresourceRange(
            {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
        ); //< Full color image range

    // Initialize pipeline stage tracking for synchronization timing
    // These stages define when operations must complete and when new operations can begin
    vk::PipelineStageFlags source_stage;      // When previous operations must finish
    vk::PipelineStageFlags destination_stage; // When subsequent operations can start

    // Configure synchronization for undefined-to-transfer layout transitions
    // This pattern is common when preparing images for data uploads
    if (old_layout == vk::ImageLayout::eUndefined &&
        new_layout == vk::ImageLayout::eTransferDstOptimal) {

        // Configure memory access permissions for upload preparation
        barrier
            .setSrcAccessMask(
                vk::AccessFlagBits::eNone
            ) // No previous access to synchronize
            .setDstAccessMask(
                vk::AccessFlagBits::eTransferWrite
            ); // Enable transfer write operations

        // Set pipeline stage synchronization points for upload workflow
        source_stage =
            vk::PipelineStageFlagBits::eTopOfPipe; // No previous work to wait for
        destination_stage =
            vk::PipelineStageFlagBits::eTransfer; // Transfer operations can proceed

        // Configure synchronization for transfer-to-shader layout transitions
        // This pattern prepares uploaded images for shader sampling
    } else if (old_layout == vk::ImageLayout::eTransferDstOptimal &&
               new_layout == vk::ImageLayout::eShaderReadOnlyOptimal) {

        // Configure memory access transition from writing to reading
        barrier
            .setSrcAccessMask(
                vk::AccessFlagBits::eTransferWrite
            ) // Previous transfer writes must complete
            .setDstAccessMask(
                vk::AccessFlagBits::eShaderRead
            ); // Enable shader read access

        // Set pipeline stage synchronization for shader usage workflow
        source_stage =
            vk::PipelineStageFlagBits::eTransfer; // Transfer operations must complete
        destination_stage =
            vk::PipelineStageFlagBits::eFragmentShader; // Fragment shaders can access

    } else {
        // Handle unsupported transition combinations
        // Production code would include additional common transition patterns
        throw std::invalid_argument("Unsupported layout transition!");
    }

    // Execute the pipeline barrier with configured synchronization
    // This commits the layout transition and memory synchronization to the command buffer
    cmdbuf.pipelineBarrier(
        source_stage,                      // Wait for these operations to complete
        destination_stage,                 // Before allowing these operations to begin
        vk::DependencyFlagBits::eByRegion, // Enable region-local optimizations
        nullptr,                           // No memory barriers needed
        nullptr,                           // No buffer barriers needed
        barrier                            // Apply our image memory barrier
    );
}

auto render_frame(
    vk::raii::Device& device, vk::raii::Queue graphics_queue,
    vk::raii::Queue present_queue
) -> void {

    // Synchronize with previous frame completion
    // Prevent CPU from submitting work faster than GPU can process it
    vk::Result result = device.waitForFences(1, &*in_flight_fence, VK_TRUE, UINT64_MAX);

    // Reset fence for this frame's completion tracking
    // Prepare the fence to signal when this frame's GPU work completes
    device.resetFences(1, &*in_flight_fence);
}

auto setup_deferred_renderer(RenderGraph& graph, uint32_t width, uint32_t height)
    -> void {

    // Configure position buffer for world-space vertex positions
    // High precision format preserves positional accuracy for lighting calculations
    graph.add_resource(
        "GBuffer_Position", vk::Format::eR16G16B16A16Sfloat, {width, height},
        vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment,
        vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal
    );

    // Configure normal buffer for surface orientation data
    // High precision normals enable accurate lighting and reflection calculations
    graph.add_resource(
        "GBuffer_Normal", vk::Format::eR16G16B16A16Sfloat, {width, height},
        vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment,
        vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal
    );

    // Configure albedo buffer for surface color information
    // Standard 8-bit precision sufficient for color data with gamma encoding
    graph.add_resource(
        "GBuffer_Albedo", vk::Format::eR8G8B8A8Unorm, {width, height},
        vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment,
        vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal
    );

    // Configure depth buffer for occlusion and depth testing
    // High precision depth enables accurate depth reconstruction in lighting pass
    graph.add_resource(
        "Depth", vk::Format::eD32Sfloat, {width, height},
        vk::ImageUsageFlagBits::eDepthStencilAttachment |
            vk::ImageUsageFlagBits::eInputAttachment,
        vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal
    );

    // Configure final color buffer for the completed lighting result
    // Standard color format with transfer capability for presentation or post-processing
    graph.add_resource(
        "FinalColor", vk::Format::eR8G8B8A8Unorm, {width, height},
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
        vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferSrcOptimal
    );

    // Configure geometry pass for G-Buffer population
    // This pass renders all geometry and stores intermediate data for lighting
    // calculations
    graph.AddPass(
        "GeometryPass", {}, // No input dependencies - first pass in pipeline
        {"GBuffer_Position", "GBuffer_Normal", "GBuffer_Albedo",
         "Depth"}, // Outputs all G-Buffer components
        [&](vk::raii::CommandBuffer& cmd) {
            // Configure multiple render target attachments for G-Buffer output
            // Each attachment corresponds to a different geometric property
            std::array<vk::RenderingAttachmentInfoKHR, 3> colour_attachments;

            // Configure position attachment - world space vertex positions
            colour_attachments[0]
                .setImageView(/* GBuffer_Position view */) // Target position buffer
                .setImageLayout(
                    vk::ImageLayout::eColorAttachmentOptimal
                )                                           // Optimal for writes
                .setLoadOp(vk::AttachmentLoadOp::eClear)    // Clear to known state
                .setStoreOp(vk::AttachmentStoreOp::eStore); // Preserve for lighting pass

            // Configure normal attachment - surface normals in world space
            colour_attachments[1]
                .setImageView(/* GBuffer_Normal view */) // Target normal buffer
                .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                .setLoadOp(vk::AttachmentLoadOp::eClear)    // Clear to default normal
                .setStoreOp(vk::AttachmentStoreOp::eStore); // Preserve for lighting

            // Configure albedo attachment - surface color and material properties
            colour_attachments[2]
                .setImageView(/* GBuffer_Albedo view */) // Target albedo buffer
                .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                .setLoadOp(vk::AttachmentLoadOp::eClear)    // Clear to default color
                .setStoreOp(vk::AttachmentStoreOp::eStore); // Preserve for lighting

            // Configure depth attachment for occlusion culling
            vk::RenderingAttachmentInfoKHR depthAttachment;
            depthAttachment
                .setImageView(/* Depth view */) // Target depth buffer
                .setImageLayout(
                    vk::ImageLayout::eDepthStencilAttachmentOptimal
                )                                          // Optimal for depth ops
                .setLoadOp(vk::AttachmentLoadOp::eClear)   // Clear to far plane
                .setStoreOp(vk::AttachmentStoreOp::eStore) // Preserve for lighting pass
                .setClearValue({1.0f, 0});                 // Clear to maximum depth

            // Assemble complete rendering configuration
            vk::RenderingInfoKHR rendering_info;
            rendering_info
                .setRenderArea({{0, 0}, {width, height}}) // Full screen rendering
                .setLayerCount(1)                         // Single layer rendering
                .setColorAttachmentCount(
                    colour_attachments.size()
                ) // Number of G-Buffer targets
                .setPColorAttachments(
                    colour_attachments.data()
                )                                       // G-Buffer attachment array
                .setPDepthAttachment(&depthAttachment); // Depth testing configuration

            // Execute geometry rendering with dynamic rendering
            cmd.beginRendering(rendering_info); // Begin G-Buffer population

            // Bind geometry pipeline and render all scene objects
            // Each draw call populates position, normal, and albedo for visible fragments
            // ... (geometry rendering implementation would go here)

            cmd.endRendering(); // Complete G-Buffer population
        }
    );

    // Configure lighting pass for screen-space illumination calculations
    // This pass reads G-Buffer data and computes final lighting for each pixel
    graph.AddPass(
        "LightingPass",
        {"GBuffer_Position", "GBuffer_Normal", "GBuffer_Albedo",
         "Depth"},      // Read all G-Buffer components
        {"FinalColor"}, // Output final lit result
        [&](vk::raii::CommandBuffer& cmd) {
            // Configure single color output for final lighting result
            vk::RenderingAttachmentInfoKHR colour_attachment;
            colour_attachment
                .setImageView(/* FinalColor view */) // Target final color buffer
                .setImageLayout(
                    vk::ImageLayout::eColorAttachmentOptimal
                )                                          // Optimal for color writes
                .setLoadOp(vk::AttachmentLoadOp::eClear)   // Clear to background color
                .setStoreOp(vk::AttachmentStoreOp::eStore) // Preserve final result
                .setClearValue({0.0f, 0.0f, 0.0f, 1.0f});  // Clear to black background

            // Configure lighting pass rendering without depth testing
            // Depth testing unnecessary since we're processing each pixel exactly once
            vk::RenderingInfoKHR rendering_info;
            rendering_info
                .setRenderArea({{0, 0}, {width, height}})  // Full screen processing
                .setLayerCount(1)                          // Single layer output
                .setColorAttachmentCount(1)                // Single color output
                .setPColorAttachments(&colour_attachment); // Final color attachment

            // Execute screen-space lighting calculations
            cmd.beginRendering(rendering_info); // Begin lighting pass

            // Bind lighting pipeline and draw full-screen quad
            // Fragment shader reads G-Buffer textures and computes lighting for each
            // pixel All scene lights are processed in a single screen-space pass
            // ... (lighting calculation implementation would go here)

            cmd.endRendering(); // Complete lighting calculations
        }
    );

    // Compile the complete rendergraph for execution
    // This analyzes dependencies and generates optimal execution plan
    graph.compile();
}

} // namespace sap::core::renderer
