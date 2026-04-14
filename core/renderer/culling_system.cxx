// <core/resources/culling_system.cxx> -*- C++ -*-

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

#include "culling_system.h"

#include <vector>

namespace sap::core::renderer {

auto CullingSystem::CullingSystem(Camera *camera) : m_camera(camera) {}

auto CullingSystem::change_camera(Camera *to_camera) -> void { m_camera = to_camera; }

auto CullingSystem::cull_scene(const std::vector<Entity *>& scene_entities) -> void {
    m_visible_entities.clear();

    if (!m_camera) {
        return;
    }

    auto frustum = m_camera->frustum();

    for (auto entity : scene_entities) {
        if (!entity->is_active()) {
            continue;
        }

        auto mesh_component = entity->get_component<MeshComponent>();

        if (!mesh_component) {
            continue;
        }

        auto transform_component = entity->get_component<TransformComponent>();

        if (!transform_component) {
            continue;
        }

        auto bounding_box = mesh_component->bounding_box();
        bounding_box.transform(transform_component->transformation_matrix());

        if (frustum.intersects(bounding_box) {
            m_visable_entities.push_back(entity);
        }
    }
}

auto CullingSystem::visable_entities() const -> const std::vector<Entity *> {
    return m_visible_entities;
}

} // namespace sap::core::renderer
