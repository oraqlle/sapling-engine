// <core/resources/lighting_render_pass.cxx> -*- C++ -*-

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

#include "lighting_render_pass.h"

namespace sap::core::renderer {

LightingRenderPass::LightingRenderPass(
    const std::string& name, GeometryRenderPass *g_pass
)
    : RenderPass(name), m_geo_pass(g_pass) {
    // Add dependency on geometry pass
    add_dependency(g_pass->name());
}

void LightingRenderPass::add_light(Light *light) { m_lights.push_back(light); }

void LightingRenderPass::remove_light(Light *light) {
    auto it = std::find(m_lights.begin(), m_lights.end(), light);
    if (it != m_lights.end()) {
        m_lights.erase(it);
    }
}

void LightingRenderPass::begin_pass(vk::raii::CommandBuffer& cmdbuf) {
    // Begin rendering with dynamic rendering
    vk::RenderingInfoKHR rendering_info;

    // Set up color attachment for the lighting pass
    vk::RenderingAttachmentInfoKHR colour_attachment;
    colour_attachment.setImageView(target()->colour_image_view())
        .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));

    // Configure rendering info
    rendering_info
        .setRenderArea(vk::Rect2D({0, 0}, {target()->width(), target()->height()}))
        .setLayerCount(1)
        .setColorAttachmentCount(1)
        .setPColorAttachments(&colour_attachment);

    // Begin dynamic rendering
    cmdbuf.beginRendering(rendering_info);
}

void LightingRenderPass::render(vk::raii::CommandBuffer& cmdbuf) {
    // Bind G-buffer textures from the geometry pass
    auto gbuffer = m_geo_pass->target();

    // Set up descriptor sets for G-buffer textures
    // With dynamic rendering, we access the G-buffer textures directly as shader
    // resources rather than as subpass inputs

    // Render full-screen quad with lighting shader
    // ...

    // For each light
    for (auto light : m_lights) {
        // Set light properties
        // ...

        // Draw light volume
        // ...
    }
}

void LightingRenderPass::end_pass(vk::raii::CommandBuffer& cmdbuf) {
    // End dynamic rendering
    cmdbuf.endRendering();
}

} // namespace sap::core::renderer
