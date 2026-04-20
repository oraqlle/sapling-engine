// <core/resources/culling_system.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_CULLING_SYSTEM_H
#define SAPLING_ENGINE_CULLING_SYSTEM_H

#include <vector>

namespace sap::core::renderer {

class CullingSystem {
private:
    // Note: Possibly use std::weak_ptr<>, std::shared_ptr or std::optional<>
    Camera *m_camera;
    std::vector<Entity *> m_visible_entities;

public:
    explicit CullingSystem(Camera *camera);

    auto change_camera(Camera *to_camera) -> void;

    auto cull_scene(const std::vector<Entity *>& entities) -> void;

    auto visible_entities() const -> const std::vector<Entity *>&;

}; // class CullingSystem

} // namespace sap::core::renderer

#endif // SAPLING_ENGINE_CULLING_SYSTEM_H
