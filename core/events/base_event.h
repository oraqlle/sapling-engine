// <core/resources/base_event.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_BASE_EVENT_H
#define SAPLING_ENGINE_BASE_EVENT_H

#include <cstdint>
#include <memory>
#include <string_view>

namespace sap::core::events {

enum class EventCategory : uint8_t {
    None = 0,
    Application = 1 << 0,
    Input = 1 << 1,
    Keyboard = 1 << 2,
    Mouse = 1 << 3,
    MouseButton = 1 << 4,
    Window = 1 << 5,
}; // enum class EventCategory

class Event {
public:
    virtual ~Event() = default;

    virtual auto type_name() const -> const std::string_view = 0;

    virtual auto clone() const -> std::unique_ptr<Event> = 0;

    virtual auto category_flags() const -> uint8_t = 0;

    auto is_in_category(const EventCategory category) const -> bool {
        return static_cast<bool>(category_flags() & static_cast<uint8_t>(category));
    }

}; // class Event

} // namespace sap::core::events

#endif // SAPLING_ENGINE_BASE_EVENT_H
