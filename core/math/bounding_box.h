// <core/math/bounding_box.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_BOUNDING_BOX_H
#define SAPLING_ENGINE_BOUNDING_BOX_H

#include <glm/glm.hpp>

namespace sap::core::math {

    class BoundingBox {
        public:
            glm::vec3 min;
            glm::vec3 max;

            BoundingBox();
            BoundingBox(
                const glm::vec3& min,
                const glm::vec3& max
            );

            auto transform(const glm::mat4& matrix) -> void;

            auto is_valid() const -> bool;

    }; // class BoundingBox

} // namespace sap::core::math

#endif // SAPLING_ENGINE_BOUNDING_BOX_H