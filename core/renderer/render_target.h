// <core/resources/render_target.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RENDER_TARGET_H
#define SAPLING_ENGINE_RENDER_TARGET_H

#include "vulkan/vulkan_raii.hpp"

namespace sap::core::renderer {

class RenderTarget {
private:
    vk::raii::Image m_colour_image = nullptr;
    vk::raii::DeviceMemory m_colour_mem = nullptr;
    vk::raii::ImageView m_colour_image_view = nullptr;

    vk::raii::Image m_depth_image = nullptr;
    vk::raii::DeviceMemory m_depth_mem = nullptr;
    vk::raii::ImageView m_depth_image_view = nullptr;

    uint32_t m_width;
    uint32_t m_height;

public:
    explicit RenderTarget(uint32_t width, uint32_t height);

    auto width() const -> uint32_t;

    auto height() const -> uint32_t;

    auto colour_image() -> vk::raii::Image;

    auto colour_mem() -> vk::raii::DeviceMemory;

    auto colour_image_view() -> vk::raii::ImageView;

    auto dpeth_image() -> vk::raii::Image;

    auto depth_mem() -> vk::raii::DeviceMemory;

    auto depth_image_view() -> vk::raii::ImageView;

private:
    auto _M_create_colour_resource() -> void;

    auto _M_create_depth_resource() -> void;

    auto _M_device() -> vk::raii::Device&;
}; // class RenderTarget

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_RENDER_TARGET_H
