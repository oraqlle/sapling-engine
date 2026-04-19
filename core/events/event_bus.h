// <core/renderer/event_bus.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_EVENT_BUS_H
#define SAPLING_ENGINE_EVENT_BUS_H

#include "base_event.h"
#include "event_listener.h"

#include <memory>
#include <mutex>
#include <queue>
#include <vector>

namespace sap::core::events {

class EventBus {
private:
    std::vector<std::weak_ptr<EventListener>> m_listeners;

    std::queue<std::unique_ptr<Event>> m_event_queue;

    std::mutex m_queue_mtx;
    bool m_immediate_mode = true;

public:
    auto set_immediate_mode(bool immediate) -> void;

    auto add_listener(std::weak_ptr<EventListener> listener) -> void;

    auto remove_listener(std::weak_ptr<EventListener> listener) -> void;

    auto publish_event(const Event& event) -> void;

    auto process_events() -> void;

private:
    auto _M_send_event_to_listeners(const Event& event) -> void;

}; // class EventBus

} // namespace sap::core::events

#endif // SAPLING_ENGINE_EVENT_BUS_H
