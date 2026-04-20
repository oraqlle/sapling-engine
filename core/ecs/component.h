// <core/ecs/component.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_COMPONENT_H
#define SAPLING_ENGINE_COMPONENT_H

namespace sap::core::ecs {

    class Entity;

    class Component {
        protected:
            Entity *m_owner = nullptr;

        public:
            Component() = default;
            virtual ~Component() = default;

            Component(const Component&) = delete;
            auto operator=(const Component&) -> Component& = delete;

            auto owner() const -> Entity * { return m_owner; }
            auto set_owner(Entity *owner) -> void { m_owner = owner; }

    }; // class Component

} // namespace::core::ecs

#endif // SAPLING_ENGINE_COMPONENT_H