// <core/resources/texture_resource.cxx> -*- C++ -*-

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

#include <texture_resource.h>

#include <vulkan/vulkan.hpp>

#include <cstddef>
#include <format>
#include <vector>

namespace sap::core::resources {

Texture::Texture(const std::string& id)
    : Resource(id) {}

Texture::~Texture() { unload(); }

auto Texture::get_image() const -> vk::Image { return m_image; }

auto Texture::get_image_view() const -> vk::ImageView { return m_image_view; }

auto Texture::get_sampler() const -> vk::Sampler { return m_sampler; }

auto Texture::do_load() -> bool {
    auto file_path = std::format("textures/{}.ktx", get_id());

    auto data = _M_load_image_data(file_path);

    if (data.empty()) {
        return false;
    }

    _M_create_vulkan_image(data);
    _M_free_image_data(data);

    return true;
}

auto Texture::do_unload() -> bool {
    if (is_loaded()) {
        auto device = _M_get_device();

        device.destroySampler(m_sampler);
        device.destroyImageView(m_image_view);
        device.destroyImage(m_image);
        device.freeMemory(m_memory);

        return true;
    }

    return false;
}

auto Texture::_M_load_image_data(const std::string& file_path)
    -> const std::vector<std::byte> {
    return std::vector<std::byte>{};
}

auto Texture::_M_free_image_data(const std::vector<std::byte> data) -> void {}

auto Texture::_M_create_vulkan_image(const std::vector<std::byte> data) -> void {}

auto Texture::_M_get_device() const -> vk::Device { return vk::Device(); }

} // namespace sap::core::resources
