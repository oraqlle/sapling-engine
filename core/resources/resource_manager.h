// <core/resources/resource_manager.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RESOURCE_MANAGER_H
#define SAPLING_ENGINE_RESOURCE_MANAGER_H

#include <base_resource.h>

#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

namespace sap::core::resources {

template <typename T>
class ResourceHandle; //< forward declare ResourceManager

class ResourceManager {
public:
    template <typename U>
    using ResourceMap =
        std::unordered_map<std::type_index, std::unordered_map<std::string, U>>;

private:
    // Two-level storage system: organize by type first, then by unique identifier
    // This approach enables type-safe resource access while maintaining efficient lookup
    ResourceMap<std::shared_ptr<Resource>> resources;

    struct ResourceData {
        std::shared_ptr<Resource> resource;
        size_t ref_count;
    }; // struct ResourceData

    // Two-level reference counting system for automatic resource lifecycle management
    // First level maps resource type, second level maps resource IDs to their data
    ResourceMap<ResourceData> ref_counts;

public:
    template <typename T>
    auto load(const std::string& resource_id) -> ResourceHandle<T> {
        static_assert(std::is_base_of_v<Resource, T>, "T must derive from Resource");

        const auto type_idx = std::type_index(typeid(T));
        auto& type_resources = resources[type_idx];

        auto it = type_resources.find(resource_id);
        if (it != type_resources.end()) {
            ref_counts[type_idx][resource_id].ref_count += 1;
            return ResourceHandle<T>(resource_id, this);
        }

        auto resource = std::make_shared<T>(resource_id);
        if (!resource->load()) {
            return ResourceHandle<T>();
        }

        type_resources[resource_id] = resource;
        ref_counts[type_idx][resource_id].ref_count = 1;

        return RourceHandle<T>(resource_id, this);
    }

    template <typename T>
    auto get_resource(const std::string& resource_id) -> T* {
        const auto type_idx = std::type_index(typeid(T));
        auto& type_resources = resources[type_idx];

        auto it = type_resources.find(resource_id);
        if (it != type_resources.end()) {
            return static_cast<T*>(it->second.get());
        }

        return nullptr;
    }

    template <typename T>
    auto has_resource(const std::string& resource_id) -> bool {
        static_assert(std::is_base_of_v<Resource, T>, "T must derive from Resource");

        auto type_it = resources.find(std::type_index(typeid(T)));
        if (type_it == resources.end()) {
            return false;
        }

        return type_it->second.contains(resource_id);
    }

    template <typename T>
    auto release(const std::string& resource_id) -> void {
        static_assert(std::is_base_of_v<Resource, T>, "T must derive from Resource");

        const auto type_idx = std::type_index(typeid(T));

        auto rc_type_it = ref_counts.find(type_idx);
        if (rc_type_it == ref_counts.end()) {
            // early return for now, logging later
            return;
        }

        auto& rc_resources = rc_type_it->second;
        auto rc_resource_it = rc_resources.find(resource_id);
        if (rc_resource_it == rc_resources.end()) {
            // early return for now, logging later
            return;
        }

        auto& rc_resource = rc_resource_it->second;
        rc_resource.ref_count -= 1;

        if (rc_resource.ref_count <= 0) {
            auto type_it = resources.find(type_idx);

            if (type_it == resources.end()) {
                // early return for now, logging later
                return;
            }

            auto& type_resources = type_it->second;
            auto resource_it = type_resources.find(resource_id);
            if (resource_it == type_resources.end()) {
                // early return for now, logging later
                return;
            }

            resource_it->second->unload();
            type_resources.erase(resource_it);

            rc_resources.erase(rc_resource_it);
        }
    }

    auto unload_all() -> void;

}; // class ResourceManager

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_RESOURCE_MANAGER_H
