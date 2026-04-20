// <core/resources/render_pass_manager.cxx> -*- C++ -*-

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

#include "render_pass_manager.h"

namespace sap::core::renderer {

RenderPass *RenderPassManager::get_render_pass(const std::string& name) {
    auto it = m_render_passes.find(name);
    if (it != m_render_passes.end()) {
        return it->second.get();
    }
    return nullptr;
}

void RenderPassManager::remove_render_pass(const std::string& name) {
    auto it = m_render_passes.find(name);
    if (it != m_render_passes.end()) {
        m_render_passes.erase(it);
        m_dirty = true;
    }
}

void RenderPassManager::execute(vk::raii::CommandBuffer& cmdbuf) {
    if (m_dirty) {
        _M_sort_passes();
        m_dirty = false;
    }

    for (auto pass : m_sorted_passes) {
        pass->execute(cmdbuf);
    }
}

void RenderPassManager::_M_sort_passes() {
    // Topologically sort render passes based on dependencies
    m_sorted_passes.clear();

    // Create a copy of render passes for sorting
    std::unordered_map<std::string, RenderPass *> pass_map;
    for (const auto& [name, pass] : m_render_passes) {
        pass_map[name] = pass.get();
    }

    // Perform topological sort
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> visiting;

    for (const auto& [name, pass] : pass_map) {
        if (visited.find(name) == visited.end()) {
            _M_topological_sort(name, pass_map, visited, visiting);
        }
    }
}

void RenderPassManager::_M_topological_sort(
    const std::string& name,
    const std::unordered_map<std::string, RenderPass *>& pass_map,
    std::unordered_set<std::string>& visited, std::unordered_set<std::string>& visiting
) {
    visiting.insert(name);

    auto pass = pass_map.at(name);
    for (const auto& dep : pass->dependencies()) {
        if (visited.find(dep) == visited.end()) {
            if (visiting.find(dep) != visiting.end()) {

                // Circular dependency detected
                throw std::runtime_error("Circular dependency detected in render passes");
            }

            _M_topological_sort(dep, pass_map, visited, visiting);
        }
    }

    visiting.erase(name);
    visited.insert(name);
    m_sorted_passes.push_back(pass);
}

} // namespace sap::core::renderer
