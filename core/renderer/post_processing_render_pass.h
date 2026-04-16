// <core/resources/post_process_render_pass.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_POST_PROCESS_RENDER_PASS_H
#define SAPLING_ENGINE_POST_PROCESS_RENDER_PASS_H

#include "lighting_render_pass.h"
#include "render_pass.h"

#include "vulkan/vulkan_raii.hpp"

#include <string>
#include <vector>

namespace sap::core::renderer {

class PostProcessEffect {
public:
    virtual ~PostProcessEffect() = default;

    virtual void apply(vk::raii::CommandBuffer& cmdbuf) = 0;
}; // class PostProcessEffect

class PostProcessRenderPass : public RenderPass {
private:
    LightingRenderPass *m_lighting_pass;
    std::vector<PostProcessEffect *> m_effects;

public:
    PostProcessRenderPass(const std::string& name, LightingRenderPass *lighting_pass);

    void add_effect(PostProcessEffect *effect);

    void remove_effect(PostProcessEffect *effect);

protected:
    void begin_pass(vk::raii::CommandBuffer& cmdbuf) override;

    void render(vk::raii::CommandBuffer& cmdbuf) override;

    void end_pass(vk::raii::CommandBuffer& cmdbuf) override;
}; // class PostProcessRenderPass

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_POST_PROCESS_RENDER_PASS_H
