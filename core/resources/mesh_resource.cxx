// <core/resources/mesh_resource.cxx> -*- C++ -*-

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

#include <mesh_resource.h>

#include "vulkan/vulkan.hpp"

#include <format>
#include <string>
#include <vector>

namespace sap::core::resources {

Mesh::Mesh(const std::string& id)
    : Resource(id) {}

Mesh::~Mesh() { unload(); }

auto Mesh::do_load() -> bool {
    auto file_path = std::format("models/{}.gltf", get_id());

    auto vertices = std::vector<Vertex>{};
    auto indices = std::vector<uint32_t>{};

    if (!_M_load_mesh_data(file_path, vertices, indices)) {
        return false;
    }

    _M_create_vertex_buffer(vertices);
    _M_create_index_buffer(indices);

    m_vertex_count = static_cast<uint32_t>(vertices.size());
    m_index_count = static_cast<uint32_t>(indices.size());

    return true;
}

auto Mesh::do_unload() -> bool {
    if (is_loaded()) {
        auto device = _M_device();

        device.destroyBuffer(m_index_buf);
        device.freeMemory(m_index_mem);

        device.destroyBuffer(m_vertex_buf);
        device.freeMemory(m_vertex_mem);

        return true;
    }

    return false;
}

auto Mesh::vertex_buffer() const -> vk::Buffer { return m_vertex_buf; }

auto Mesh::index_buffer() const -> vk::Buffer { return m_index_buf; }

auto Mesh::vertex_count() const -> uint32_t { return m_vertex_count; }

auto Mesh::index_count() const -> uint32_t { return m_index_count; }

auto Mesh::_M_load_mesh_data(
    const std::string& file_path, std::vector<Vertex>& vertices,
    std::vector<uint32_t> indices
) -> bool {
    return true;
}

auto Mesh::_M_create_vertex_buffer(const std::vector<Vertex>& vertices) -> void {}

auto Mesh::_M_create_index_buffer(const std::vector<uint32_t>& indices) -> void {}

auto Mesh::_M_device() -> vk::Device { return vk::Device(); }

} // namespace sap::core::resources
