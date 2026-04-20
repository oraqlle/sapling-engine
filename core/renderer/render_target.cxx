// <core/resources/render_target.cxx> -*- C++ -*-

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

#include "render_target.h"

namespace sap::core::renderer {

RenderTarget::RenderTarget(uint32_t width, uint32_t height)
    : m_width(width), m_height(height) {

    // Create color and depth images
    _M_create_colour_resource();
    _M_create_depth_resource();

    // Note: With dynamic rendering, we don't need to create VkRenderPass
    // or VkFramebuffer objects. Instead, we just create the images and
    // image views that will be used directly with vkCmdBeginRendering.
}

vk::ImageView RenderTarget::colour_image_view() const { return *m_colour_image_view; }

vk::ImageView RenderTarget::depth_image_view() const { return *m_depth_image_view; }

uint32_t RenderTarget::width() const { return m_width; }

uint32_t RenderTarget::height() const { return m_height; }

void RenderTarget::_M_create_colour_resource() {
    // Implementation to create color image, memory, and view
    // With dynamic rendering, we just need to create the image and image view
    // that will be used with vkCmdBeginRendering
    // ...
}

void RenderTarget::_M_create_depth_resource() {
    // Implementation to create depth image, memory, and view
    // With dynamic rendering, we just need to create the image and image view
    // that will be used with vkCmdBeginRendering
    // ..
}

vk::raii::Device& RenderTarget::_M_device() {
    // Get device from somewhere (e.g., singleton or parameter)
    // ...
    static vk::raii::Device device = nullptr; // Placeholder
    return device;
}

} // namespace sap::core::renderer
