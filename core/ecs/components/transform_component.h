// <core/ecs/components/transform_component.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_TRANSFORM_COMPONENT_H
#define SAPLING_ENGINE_TRANSFORM_COMPONENT_H

#include "../component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace sap::core::ecs {

    class TransformComponent : public Component {
        private:
            glm::vec3 m_position;
            glm::quat m_rotation;
            glm::vec3 m_scale;

        public:
            TransformComponent();
            TransformComponent(
                const glm::vec3& position,
                const glm::quat& rotation,
                const glm::vec3& scale
            );
            ~TransformComponent() override = default;

            auto position() const -> const glm::vec3& { return m_position; }
            auto rotation() const -> const glm::quat& { return m_rotation; }
            auto scale() const -> const glm::vec3& { return m_scale; }

            auto set_position(const glm::vec3& position) -> void {m_position = position; }
            auto set_rotation(const glm::quat& rotation) -> void { m_rotation = rotation; }
            auto set_scale(const glm::vec3& scale) -> void { m_scale = scale; }

            auto transformation_matrix() const -> glm::mat4;

    }; // class TransformComponent

} // namespace sap::core::ecs

#endif // SAPLING_ENGINE_TRANSFORM_COMPONENT_H