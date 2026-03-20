// <core/resources/mesh_resource.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_MESH_RESOURCE_H
#define SAPLING_ENGINE_MESH_RESOURCE_H

#include <base_resource.h>
#include <resource_handle.h>
#include <resource_manager.h>

#include "vulkan/vulkan.hpp"

#include <string>

namespace sap::core::resources {

struct Vertex {
    float w = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
}; // struct Vertex

class Mesh : public Resource {
private:
    // Vertex data management - stores per-vertex attributes like position, normal,
    // UV coordinates
    uint32_t m_vertex_count = 0;    // Number of vertices in this mesh
    vk::Buffer m_vertex_buf;        // GPU buffer containing vertex attribute data
    vk::DeviceMemory m_vertex_mem;  // GPU memory backing the vertex buffer
    vk::DeviceSize m_vertex_offset; // Offset within the memory allocation for
                                    // vertex buffer

    // Index data management - defines triangle connectivity using vertex indices
    uint32_t m_index_count = 0;    // Number of indices in mesh (typically 3 per triangle)
    vk::Buffer m_index_buf;        // GPU buffer containing triangle index data
    vk::DeviceMemory m_index_mem;  // GPU memory backing the index buffer
    vk::DeviceSize m_index_offset; // Offset within the memory allocation for
                                   // index buffer

public:
    explicit Mesh(const std::string& id);

    ~Mesh() override;

    auto vertex_buffer() const -> vk::Buffer;

    auto index_buffer() const -> vk::Buffer;

    auto vertex_count() const -> uint32_t;

    auto index_count() const -> uint32_t;

protected:
    bool do_load() override;
    bool do_unload() override;

private:
    auto _M_load_mesh_data(
        const std::string& file_path, std::vector<Vertex>& vertices,
        std::vector<uint32_t> indices
    ) -> bool;

    auto _M_create_vertex_buffer(const std::vector<Vertex>& vertices) -> void;

    auto _M_create_index_buffer(const std::vector<uint32_t>& indices) -> void;

    auto _M_device() -> vk::Device;

}; // class Mesh

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_MESH_RESOURCE_H
