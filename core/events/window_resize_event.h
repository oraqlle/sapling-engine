// <core/resources/window_resize_event.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_WINDOW_RESIZE_EVENT_H
#define SAPLING_ENGINE_WINDOW_RESIZE_EVENT_H

#include "base_event.h"

#include <cstdint>
#include <string_view>

namespace sap::core::events {

class WindowResizeEvent : public Event {
private:
    uint32_t m_width;
    uint32_t m_height;

public:
    explicit WindowResizeEvent(uint32_t width, uint32_t height);

    auto width() const -> uint32_t;

    auto height() const -> uint32_t;

    auto clone() const -> std::unique_ptr<Event> override;

    auto type_name() const -> const std::string_view override;

    auto category_flags() const -> uint8_t override;

    static auto static_type_name() -> const std::string_view;

}; // class Event

} // namespace sap::core::events

#endif // SAPLING_ENGINE_WINDOW_RESIZE_EVENT_H
