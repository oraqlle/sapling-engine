// <core/ecs/components/transform_component.cxx> -*- C++ -*-

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

#include "transform_component.h"

#include <glm/ext/matrix_transform.hpp>

namespace sap::core::ecs {

    TransformComponent::TransformComponent()
        : m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(1.0, 0.0f, 0.0f, 0.0f)
        , m_scale(1.0f, 1.0f, 1.0f) {}

    TransformComponent::TransformComponent (
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& scale
    )
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale) {}

    auto TransformComponent::transformation_matrix() const -> glm::mat4 {
        auto translation = glm::translate(glm::mat4(1.0f), m_position);
        auto rotation = glm::mat4_cast(m_rotation);
        auto scaling = glm::scale(glm::mat4(1.0f), m_scale);

        return translation * rotation * scaling;
    }

} // namespace sap::core::ecs