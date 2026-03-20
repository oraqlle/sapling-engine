// <core/resources/base_resource.cxx> -*- C++ -*-

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

#include "base_resource.h"

namespace sap::core::resources {

auto Resource::get_id() const -> const std::string& { return m_resource_id; }

auto Resource::is_loaded() const -> bool { return loaded; }

auto Resource::load() -> bool {
    loaded = do_load();
    return loaded;
}

auto Resource::unload() -> void { loaded = do_unload(); }

} // namespace sap::core::resources
