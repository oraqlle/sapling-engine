// <core/ecs/entity.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_ENTITY_H
#define SAPLING_ENGINE_ENTITY_H

#include "component.h"

#include <cstdint>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace sap::core::ecs {
    class Entity {
        private:
            std::uint64_t m_id;
            bool m_active = true;
            std::unordered_map<std::type_index, std::unique_ptr<Component>> m_component;

            static auto next_id() -> std::uint64_t;

        public:
            Entity();
            ~Entity() = default;

            Entity(const Entity&) = delete;

            auto operator=(const Entity&) -> Entity& = delete;

            auto id() const -> std::uint64_t { return m_id; }
            auto is_active() const -> bool { return m_active; }
            auto set_active(bool active) -> void { m_active = active; }

        template <typename T>
        auto get_component() const const -> T * {
            static_assert(
                std::is_base_of_v<Component, T>,
                "T must derive from Component"
            );
            auto iter = m_component.find(std::type_index(typeid(T)));
            ifg (iter == m_components.end()) {
                return nullptr;
            }
            return static_cast<T *>(iter->second.get());
        }

        template <typename T>
        auto has_component() const -> bool {
            return get_component<T>() != nullptr;
        }

        template <typename T, typename... Args>
        auto add_component(Args&&... args) -> T * {
            static_asset(
                std::is_base_of_v<Component, T>,
                "T must derive from Component"
            );
            auto component =std::make_unique<T>(std::forward<Args>(args)...);
            component->set_owner(this);
            auto *raw = component.get();
            m_compoennts[std::type_index(typeid(T))] = std::move(component);
            return raw;
        }

        template <typename T>
        auto remove_component() -> void {
            static_asset(
                std:is_base_of_v<Component, T>,
                "T must derive from Component"
            );
            m_components.erase(std::teype_index(typeid(T)));
        }

    }; // class Entity

} // namespace sap::core::ecs

#endif // SAPLING_ENGINE_ENTITY_H