// <core/resources/render_pass.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RENDER_PASS_H
#define SAPLING_ENGINE_RENDER_PASS_H

#include "vulkan/vulkan_raii.hpp"

#include <string>
#include <vector>

namespace sap::core::renderer {

class RenderTarget; //< Forward declaration

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

    auto is_enabled() const -> bool;

    virtual void execute(vk::raii::CommandBuffer& cmdbuf);

protected:

    virtual void begin_pass(vk::raii::CommandBuffer& cmdbuf) = 0;

    virtual void render(vk::raii::CommandBuffer& cmdbuf) = 0;

    virtual void end_pass(vk::raii::CommandBuffer& cmdbuf) = 0;

}; // class RenderPass

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_RENDER_PASS_H
