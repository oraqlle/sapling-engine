// <core/renderer/event_listener.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_EVENT_LISTENER_H
#define SAPLING_ENGINE_EVENT_LISTENER_H

#include "base_event.h"

namespace sap::core::events {

class EventListener {
public:
    virtual ~EventListener() = default;

    virtual auto on_event(const Event& event) -> void = 0;

}; // class EventListener

} // namespace sap::core::events

#endif // SAPLING_ENGINE_EVENT_LISTENER_H
