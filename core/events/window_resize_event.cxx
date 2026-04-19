// <core/resources/window_resize_event.cxx> -*- C++ -*-

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

#include "window_resize_event.h"

#include <string_view>

namespace sap::core::events {

WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height) {}

auto WindowResizeEvent::width() const -> uint32_t { return m_width; }

auto WindowResizeEvent::height() const -> uint32_t { return m_height; }

auto WindowResizeEvent::clone() const -> std::unique_ptr<Event> {
    return std::make_unique<WindowResizeEvent>(*this);
};

auto WindowResizeEvent::type_name() const -> const std::string_view {
    return "WindowResizeEvent";
}

auto WindowResizeEvent::category_flags() const -> uint8_t {
    return static_cast<uint8_t>(EventCategory::Application) |
           static_cast<uint8_t>(EventCategory::Window);
}

auto WindowResizeEvent::static_type_name() -> const std::string_view {
    return "WindowResizeEvent";
}

} // namespace sap::core::events
