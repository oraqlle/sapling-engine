// <core/math/bounding_box.cxx> -*- C++ -*-

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

#include "bounding_box.h"

#include <array>
#include <limits>

namespace sap::core::math {

    // make empty box invalid (max > min)
    BoundingBox::BoundingBox()
        : min(std::numeric_limits<float>::max())
        , max(std::numeric_limits<float>::lowest()) {}

    BoundingBox::BoundingBox(
        const glm::vec3& min,
        const glm::vec3& max
    )
        : min(min)
        , max(max) {}

    auto BoundingBox::transform(const glm::mat4& matrix) -> void {
        const auto corners = std::array<glm::vec3, 8> {
            glm::vec3{min.x, min.y, min.z},
            glm::vec3{max.x, min.y, min.z},
            glm::vec3{min.x, max.y, min.z},
            glm::vec3{min.x, min.y, max.z},
            glm::vec3{max.x, max.y, min.z},
            glm::vec3{max.x, min.y, max.z},
            glm::vec3{min.x, max.y, max.z},
            glm::vec3{max.x, max.y, max.z}
        };

        auto new_min = glm::vec3{std::numeric_limits<float>::max()};
        auto new_max = glm::vec3{std::numeric_limits<float>::lowest()};

        for (const auto& corner : corners) {
            const auto transformed = glm::vec3{matrix * glm::vec4{corner, 1.0f}};
            new_min = glm::min(new_min, transformed);
            new_max = glm::max(new_max, transformed);
        }

        min = new_min;
        max = new_max;
    }

    auto BoundingBox::is_valid() const -> bool {
        return 
            min.x <= max.x &&
            min.y <= max.y &&
            min.z <= max.z;
    }

} // namespace sap::core::math