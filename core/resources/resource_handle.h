// <core/resources/resource_handle.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_RESOURCE_HANDLE_H
#define SAPLING_ENGINE_RESOURCE_HANDLE_H

#include <string>

namespace sap::core::resources {

class ResourceManager; //< forward declare ResourceManager

template <typename T>
class ResourceHandle {
private:
    std::string m_resource_id;
    ResourceManager *m_resource_manager;

public:
    ResourceHandle()
        : m_resource_manager(nullptr) {}

    ResourceHandle(const std::string& id, ResourceManager *manager)
        : m_resource_id(id)
        , m_resource_manager(manager) {}

    auto get() const -> T * {
        if (!m_resource_manager) {
            return nullptr;
        }

        return m_resource_manager->get_resource<T>(m_resource_id);
    }

    auto is_valid() const -> bool {
        return m_resource_manager && m_resource_manager->has_resource<T>(m_resource_id);
    }

    auto id() const -> const std::string& { return m_resource_id; }

    auto operator->() const -> T * { return get(); }

    auto operator*() const -> T& { return *get(); }

    operator bool() const { return is_valid(); }
}; // class ResourceHandle

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_RESOURCE_HANDLE_H
