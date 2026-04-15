// <core/resources/render_pass.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RENDER_PASS_H
#define SAPLING_ENGINE_RENDER_PASS_H

#include "vulkan/vulkan_raii.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class RenderPass;   //< Forward declaration
class RenderTarget; //< Forward declaration

namespace sap::core::renderer {

class RenderPassManager {
private:
    std::unordered_map<std::string, std::unique_ptr<RenderPass>> m_render_passes;
    std::vector<RenderPass *> m_sorted_passes;
    bool m_dirty;

public:
    template <typename T, typename... Args>
    auto add_render_pass(const std::string& name, Args... args) -> T * {}

    auto get_render_pass(const std::string& name) -> RenderPass *;

    auto remove_render_pass(const std::string& name) -> void;

    auto execute(vk::raii::CommandBuffer& cmdbuf) -> void;

private:
    auto _M_sort_passes() -> void;

    auto _M_topological_sort(
        const std::string& name,
        const std::unordered_map<std::string, RenderPass *>& pass_map,
        std::unordered_set<std::string>& visited,
        std::unordered_set<std::string>& visiting
    ) -> void;

}; // class RenderPassManager

class RenderPass {
private:
    std::string m_name;
    std::vector<std::string> m_deps;
    RenderTarget* m_target = nullptr;
    bool m_enabled = true;

public:
    explicit RenderPass(const std::string& name);

    virtual ~RenderPass() = default;

    auto name() const -> const std::string&;

    auto add_dependency(const std::string& dependency) -> void;

    auto dependencies() const -> const std::vector<std::string>&;

    auto set_render_target(RenderTarget *target) -> void;

    auto target() const -> RenderTarget*;
    
    auto enable_as(bool enable) -> void;

    auto is_enabled() -> bool;

    virtual void execute(vk::raii::CommandBuffer& cmdbuf);

protected:

    virtual void begin_pass(vk::raii::CommandBuffer& cmdbuf) = 0;

    virtual void render(vk::raii::CommandBuffer& cmdbuf) = 0;

    virtual void end_pass(vk::raii::CommandBuffer& cmdbuf) = 0;

}; // class RenderPass

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

#endif // SAPLING_ENGINE_RENDER_PASS_H
