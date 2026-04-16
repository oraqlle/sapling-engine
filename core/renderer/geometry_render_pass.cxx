// <core/resources/geometry_render_pass.cxx> -*- C++ -*-

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

#include "geometry_render_pass.h"

#include "culling_system.h"

namespace sap::core::renderer {

GeometryRenderPass::GeometryRenderPass(
    const std::string& name, CullingSystem *culling_sys
)
    : RenderPass(name), m_culling_sys(culling_sys) {
    // Create G-buffer render target
    m_gbuffer = new RenderTarget(1920, 1080); // Example resolution
    set_render_target(m_gbuffer);
}

GeometryRenderPass::~GeometryRenderPass() { delete m_gbuffer; }

void GeometryRenderPass::begin_pass(vk::raii::CommandBuffer& cmdbuf) {
    // Begin rendering with dynamic rendering
    vk::RenderingInfoKHR rendering_info;

    // Set up color attachment
    vk::RenderingAttachmentInfoKHR colour_attachment;
    colour_attachment.setImageView(m_gbuffer->colour_image_view())
        .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));

    // Set up depth attachment
    vk::RenderingAttachmentInfoKHR depth_attachment;
    depth_attachment.setImageView(m_gbuffer->depth_image_view())
        .setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setClearValue(vk::ClearDepthStencilValue(1.0f, 0));

    // Configure rendering info
    rendering_info
        .setRenderArea(vk::Rect2D({0, 0}, {m_gbuffer->width(), m_gbuffer->height()}))
        .setLayerCount(1)
        .setColorAttachmentCount(1)
        .setPColorAttachments(&colour_attachment)
        .setPDepthAttachment(&depth_attachment);

    // Begin dynamic rendering
    cmdbuf.beginRendering(rendering_info);
}

void GeometryRenderPass::render(vk::raii::CommandBuffer& cmdbuf) {
    // Get visible entities
    const auto& visible_entities = m_culling_sys->visible_entities();

    // Render each entity to G-buffer
    for (auto entity : visible_entities) {
        auto mesh_component = entity->get_component<MeshComponent>();
        auto transform_component = entity->get_component<TransformComponent>();

        if (mesh_component && transform_component) {
            // Bind pipeline for G-buffer rendering
            // ...

            // Set model matrix
            // ...

            // Draw mesh
            // ...
        }
    }
}

void GeometryRenderPass::end_pass(vk::raii::CommandBuffer& cmdbuf) {
    // End dynamic rendering
    cmdbuf.endRendering();
}

} // namespace sap::core::renderer
