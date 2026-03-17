// <core/resources/resource_manager.cxx> -*- C++ -*-

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

#include <resource_manager.h>

namespace sap::core::resources {

auto ResourceManager::unload_all() -> void {
    for (auto& [type, type_resources] : resources) {
        for (auto& [id, resource] : type_resources) {
            resource->unload();
        }

        type_resources.clear();
    }

    ref_counts.clear();
}

} // namespace sap::core::resources
