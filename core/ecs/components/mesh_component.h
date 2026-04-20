// <core/ecs/components/mesh_component.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_MESH_COMPONENT_H
#define SAPLING_ENGINE_MESH_COMPONENT_H

#include "../component.h"

#include <core/math/bounding_box.h>

namespace sap::core::ecs {

    class MeshComponent : public Component {
        private:
            sap::core::math::BoundingBox m_bounding_box;

        public:
            MeshComponent();
            explicit MeshComponent(const sap::core::math::BoundingBox& bounding_box);
            ~MeshComponent() override = default;

            auto bounding_box() const -> sap::core::math::BoundingBox {
                return m_bounding_box;
            }

            auto set_bounding_box(const sap::core::math::BoundingBox& bounding_box) -> void {
                m_bounding_box = bounding_box;
            }

    }; // class MeshComponent

} // namespace sap::core::ecs

#endif // SAPLING_ENGINE_COMPONENT_H