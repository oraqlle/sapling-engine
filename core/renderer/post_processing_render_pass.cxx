// <core/resources/post_processing_render_pass.cxx> -*- C++ -*-

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

#include "post_processing_render_pass.h"

#include <string>

namespace sap::core::renderer {

PostProcessRenderPass::PostProcessRenderPass(const std::string& name, LightingRenderPass *lighting_pass)
    : RenderPass(name), m_lighting_pass(lighting_pass) {
    add_dependency(m_lighting_pass->name());
}

void PostProcessRenderPass::add_effect(PostProcessEffect *effect) { m_effects.push_back(effect); }

void PostProcessRenderPass::remove_effect(PostProcessEffect *effect) {
    auto it = std::find(m_effects.begin(), m_effects.end(), effect);
    if (it != m_effects.end()) {
        m_effects.erase(it);
    }
}

void PostProcessRenderPass::begin_pass(vk::raii::CommandBuffer& cmdbuf) {
    // Begin rendering with dynamic rendering
    vk::RenderingInfoKHR rendering_info;

    // Set up color attachment for the post-processing pass
    vk::RenderingAttachmentInfoKHR colour_attachment;
    colour_attachment.setImageView(target()->get_colour_image_view())
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

void PostProcessRenderPass::render(vk::raii::CommandBuffer& cmdbuf) {
    // With dynamic rendering, each effect can set up its own rendering state
    // and access input textures directly as shader resources

    // Apply each post-process effect
    for (auto effect : m_effects) {
        effect->apply(cmdbuf);
    }
}

void PostProcessRenderPass::end_pass(vk::raii::CommandBuffer& cmdbuf) {
    // End dynamic rendering
    cmdbuf.endRendering();
}

} // namespace sap::core::renderer
