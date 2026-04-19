// <core/renderer/event_bus.cxx> -*- C++ -*-

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

#include "event_bus.h"
#include "base_event.h"
#include "event_listener.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

namespace sap::core::events {

auto EventBus::set_immediate_mode(bool immediate) -> void {
    m_immediate_mode = immediate;
}

auto EventBus::add_listener(
    std::weak_ptr<EventListener> listener, int8_t category_filter = -1,
    uint32_t priority = 0
) -> void {

    m_listeners.push_back({
        .listener = listener,
        .category_filter = category_filter,
        .priority = priority
    });
}

auto EventBus::remove_listener(std::weak_ptr<EventListener> listener) -> void {
    auto it = std::find_if(
        m_listeners.begin(), m_listeners.end(),
        [&listener](const std::weak_ptr<EventListener>& other) {
            if (other.expired()) {
                return false;
            }

            return !listener.owner_before(other) && !other.owner_before(listener);
        }
    );

    if (it != m_listeners.end()) {
        m_listeners.erase(it);
    }
}

auto EventBus::publish_event(const Event& event) -> void {
    if (m_immediate_mode) {
        _M_send_event_to_listeners(event);
    } else {
        auto lock = std::lock_guard<std::mutex>(m_queue_mtx);
        m_event_queue.push(event.clone());
    }
}

auto EventBus::process_events() -> void {
    if (m_immediate_mode) {
        return;
    }

    auto current_events = std::queue<std::unique_ptr<Event>>();

    {
        auto lock = std::lock_guard<std::mutex>(m_queue_mtx);
        std::swap(current_events, m_event_queue);
    }

    while (!current_events.empty()) {
        auto& event = *current_events.front();
        _M_send_event_to_listeners(event);
        current_events.pop();
    }
}

auto EventBus::_M_send_event_to_listeners(const Event& event) -> void {
    for (auto listener : m_listeners) {
        if (listener.expired()) {
            // TODO: Better logging system and possibly erase weak_ptr
            std::clog << "[Event System]: Listener expired\n";
        } else {
            auto lst = listener.lock();
            lst->on_event(event);
        }
    }
}

} // namespace sap::core::events
