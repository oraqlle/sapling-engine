// <core/resources/texture_resource.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_TEXTURE_RESOURCE_H
#define SAPLING_ENGINE_TEXTURE_RESOURCE_H

#include "base_resource.h"
#include "resource_handle.h"
#include "resource_manager.h"

#include "vulkan/vulkan.hpp"

#include <cstddef>
#include <vector>

namespace sap::core::resources {

class Texture : public Resource {
private:
    // Vulkan GPU objects
    vk::Image m_image;          // GPU object containing pixel data
    vk::DeviceMemory m_memory;  // GPU memory storing image
    vk::DeviceSize m_offset;    // Offset within memory allocation containing image
    vk::ImageView m_image_view; // Shader-accessible view of image
    vk::Sampler m_sampler;      // Sampling config for filtering, wrapping etc.

    // Texture metadata for validation and debugging
    size_t m_width = 0;    // Image width in pixels
    size_t m_height = 0;   // Image height in pixels
    size_t m_channels = 0; // Number of colour channels (RGB=3, RGBA=4)

public:
    explicit Texture(const std::string& id);

    ~Texture() override;

    auto image() const -> vk::Image;

    auto image_view() const -> vk::ImageView;

    auto sampler() const -> vk::Sampler;

protected:
    bool do_load() override;

    bool do_unload() override;

private:
    auto _M_load_image_data(const std::string& file_path) -> const std::vector<std::byte>;

    auto _M_free_image_data(const std::vector<std::byte> data) -> void;

    auto _M_create_vulkan_image(const std::vector<std::byte> data) -> void;

    auto _M_get_device() const -> vk::Device;

}; // class Texture

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_TEXTURE_RESOURCE_H
