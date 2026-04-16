// <core/resources/render_pass.cxx> -*- C++ -*-

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

#include "render_pass.h"

namespace sap::core::renderer {

const std::string& RenderPass::name() const { return m_name; }

void RenderPass::add_dependency(const std::string& dependency) {
    m_deps.push_back(dependency);
}

const std::vector<std::string>& RenderPass::dependencies() const { return m_deps; }

void RenderPass::set_render_target(RenderTarget *target) { m_target = target; }

RenderTarget *RenderPass::target() const { return m_target; }

void RenderPass::enable_as(bool enable) { m_enabled = enable; }

bool RenderPass::is_enabled() const { return m_enabled; }

void RenderPass::execute(vk::raii::CommandBuffer& cmdbuf) {
    if (!m_enabled)
        return;

    begin_pass(cmdbuf);
    render(cmdbuf);
    end_pass(cmdbuf);
}

} // namespace sap::core::renderer
