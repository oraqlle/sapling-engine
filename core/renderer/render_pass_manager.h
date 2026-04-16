// <core/resources/render_pass_manager.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RENDER_PASS_MANAGER_H
#define SAPLING_ENGINE_RENDER_PASS_MANAGER_H

#include "render_pass.h"

#include "vulkan/vulkan_raii.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sap::core::renderer {

class RenderPass; //< Forward declaration

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

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_RENDER_PASS_MANAGER_H
