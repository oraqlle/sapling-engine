// <core/resources/geometry_render_pass.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_GEOMETRY_RENDER_PASS_H
#define SAPLING_ENGINE_GEOMETRY_RENDER_PASS_H

#include "culling_system.h"
#include "render_pass.h"
#include "render_target.h"

#include "vulkan/vulkan_raii.hpp"

#include <string>

namespace sap::core::renderer {

class GeometryRenderPass : public RenderPass {
private:
    CullingSystem *m_culling_sys;

    // Possible std::unique_ptr<> or std::shared_ptr<>
    RenderTarget *m_gbuffer;

public:
    explicit GeometryRenderPass(const std::string& name, CullingSystem *culling_sys);

    ~GeometryRenderPass() override;

    void execute(vk::raii::CommandBuffer& cmdbuf) override;

protected:
    void begin_pass(vk::raii::CommandBuffer& cmdbuf) override;

    void render(vk::raii::CommandBuffer& cmdbuf) override;

    void end_pass(vk::raii::CommandBuffer& cmdbuf) override;

}; // class GeometryRenderPass

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_GEOMETRY_RENDER_PASS_H
