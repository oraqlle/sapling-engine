// <core/resources/keypress_event.cxx> -*- C++ -*-

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

#include "keypress_event.h"

#include <memory>
#include <string_view>

namespace sap::core::events {

KeypressEvent::KeypressEvent(uint32_t key, bool repeat)
    : m_keycode(key), m_repeat(repeat) {}

auto KeypressEvent::keycode() const -> uint32_t { return m_keycode; }

auto KeypressEvent::repeated() const -> uint32_t { return m_repeat; }

auto KeypressEvent::type_name() const -> const std::string_view {
    return "KeypressEvent";
}

auto KeypressEvent::clone() const -> std::unique_ptr<Event> {
    return std::make_shared<KeypressEvent>(*this);
};

auto KeypressEvent::static_type_name() -> const std::string_view {
    return "KeypressEvent";
}

} // namespace sap::core::events
