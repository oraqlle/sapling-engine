// <core/resources/render_graph.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RENDER_GRAPH_H
#define SAPLING_ENGINE_RENDER_GRAPH_H

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

#include <functional>
#include <unordered_map>
#include <utility>

#include <string>
#include <vector>

namespace sap::core::renderer {

class RenderGraph {
private:
    // clang-format off

    // Represents Image resource used during rendering (textures)
    struct Resource {
        std::string name;               // Human-readable identifier for debugging and referencing
        vk::Format format;              // Pixel format (RGBA8, Depth24Stencil8, etc.)
        vk::Extent2D extent;            // Dimensions in pixels for 2D resources
        vk::ImageUsageFlags usage;      // How this resource will be used (color attachment, texture, etc.)
        vk::ImageLayout initial_layout; // Expected layout when the frame begins
        vk::ImageLayout final_layout;   // Required layout when the frame ends

        // Actual GPU resources - populated during compilation
        vk::raii::Image image = nullptr;         // The GPU image object
        vk::raii::DeviceMemory memory = nullptr; // Backing memory allocation
        vk::raii::ImageView view = nullptr;      // Shader-accessible view of the image
    }; // struct Resource

    // Each pass represents a distinct rendering operation
    // with defined inputs and outputs
    struct Pass {
        std::string name;                                           // Descriptive name for debugging and profiling
        std::vector<std::string> inputs;                            // Resources this pass reads from (dependencies)
        std::vector<std::string> outputs;                           // Resources this pass writes to (products)
        std::function<void(vk::raii::CommandBuffer&)> execute_func; // The actual rendering code
    }; // struct Pass

    // clang-format on

    std::unordered_map<std::string, Resource> m_resources;
    std::vector<Pass> m_passes;
    std::vector<size_t> m_execution_order;

    std::vector<vk::raii::Semaphore> m_semaphores;
    std::vector<std::pair<size_t, size_t>> m_semaphore_signal_wait_pairs;

    vk::raii::Device& m_device;

public:
    explicit RenderGraph(vk::raii::Device& device);

    /**
     * @brief Resource registration interface for declaring all resources used during
     * rendering. This method establishes resource metadata without creating actual GPU
     * resources
     */
    auto add_resource(
        const std::string& name, vk::Format format, vk::Extent2D extent,
        vk::ImageUsageFlags usage, vk::ImageLayout initial_layout,
        vk::ImageLayout final_layout
    ) -> void;

    /**
     * @brief
     */
    auto add_pass(
        const std::string& name, const std::vector<std::string>& inputs,
        const std::vector<std::string>& outputs,
        std::function<void(vk::raii::CommandBuffer&)> execute_func
    ) -> void;

    auto get_resource(const std::string& name) -> Resource *;

    /**
     * Rendergraph compilation - transforms declarative descriptions into executable
     * pipeline. This method performs dependency analysis, resource allocation, and
     * execution planning
     */
    auto compile() -> void;

    /**
     * Rendergraph execution engine - coordinates pass execution with automatic
     * synchronization. This method transforms the compiled rendergraph into actual GPU
     * work
     */
    auto execute(vk::raii::CommandBuffer& cmdbuf, vk::Queue queue) -> void;

private:
    /**
     * @brief
     */
    auto _M_find_memory_type(uint32_t type_filter, vk::MemoryPropertyFlags props)
        -> uint32_t;

}; // class RenderGraph

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_RENDER_GRAPH_H
