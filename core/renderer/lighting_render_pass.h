// <core/resources/lighting_render_pass.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_LIGHTING_RENDER_PASS_H
#define SAPLING_ENGINE_LIGHTING_RENDER_PASS_H

#include "geometry_render_pass.h"
#include "render_pass.h"

#include "vulkan/vulkan_raii.hpp"

#include <string>

namespace sap::core::renderer {

class LightingRenderPass : public RenderPass {
private:
    GeometryRenderPass *m_geo_pass;
    std::vector<Light *> m_lights;

public:
    explicit LightingRenderPass(const std::string& name, GeometryRenderPass *geo_pass);

    void add_light(Light *light);

    void remove_light(Light *light);

protected:
    void begin_pass(vk::raii::CommandBuffer& cmdbuf) override;

    void render(vk::raii::CommandBuffer& cmdbuf) override;

    void end_pass(vk::raii::CommandBuffer& cmdbuf) override;
}; // class LightingRenderPass

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_LIGHTING_RENDER_PASS_H
