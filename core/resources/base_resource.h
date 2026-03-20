// <core/resources/base_resource.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_BASE_RESOURCE_H
#define SAPLING_ENGINE_BASE_RESOURCE_H

#include <string>

namespace sap::core::resources {

class Resource {
private:
    std::string m_resource_id;
    bool loaded = false;

protected:
    explicit Resource(const std::string& id)
        : m_resource_id(id) {}

public:
    virtual ~Resource() = default;

    auto get_id() const -> const std::string&;

    auto is_loaded() const -> bool;

    auto load() -> bool;

    auto unload() -> void;

protected:
    virtual auto do_load() -> bool = 0;
    virtual auto do_unload() -> bool = 0;

}; // class Resource

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_BASE_RESOURCE_H
