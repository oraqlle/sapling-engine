// <core/renderer/renderer_pipeline.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RENDERER_PIPELINE_H
#define SAPLING_ENGINE_RENDERER_PIPELINE_H

#include "render_graph.h"

#include "vulkan/vulkan_raii.hpp"

/**
 * @brief
 */
auto transition_image_layout(
    vk::raii::CommandBuffer& cmdbuf, vk::Image image, vk::Format format,
    vk::ImageLayout old_layout, vk::ImageLayout new_layout
) -> void;

/**
 * @brief Comprehensive frame rendering with proper synchronization. This function
 * demonstrates the complete cycle of frame rendering coordination
 */
auto render_frame(
    vk::raii::Device& device, vk::raii::Queue graphics_queue,
    vk::raii::Queue present_queue
) -> void;

/**
 * @brief Comprehensive deferred renderer setup demonstrating rendergraph resource
 * management. This implementation shows how to efficiently organize multi-pass rendering
 * workflows
 */
auto setup_deferred_renderer(RenderGraph& graph, uint32_t width, uint32_t height) -> void;

class Renderer {}; // class Renderer

#endif // SAPLING_ENGINE_RENDERER_H
