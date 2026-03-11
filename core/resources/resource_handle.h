// <core/resources.resource_handle.h> -*- C++ -*-

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

#include <resource_manager.h>

#include <string>

namespace sap::core::resources {

template<typename T>
class ResourceHandle {
private:
    std::string m_resource_id;
    ResourceManager *m_resource_manager;

    auto _M_internal_method() -> void;
};

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_RESOURCE_HANDLE_H
