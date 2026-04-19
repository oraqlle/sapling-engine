// <core/renderer/event_dispatcher.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_EVENT_DISPATCHER_H
#define SAPLING_ENGINE_EVENT_DISPATCHER_H

#include "base_event.h"

namespace sap::core::events {

template <typename U> class EventDispatcher {
private:
    const Event<U>& m_event;

public:
    explicit EventDispatcher(const Event<U> event) : m_event(event) {}

    template <typename T, typename F> auto dispatch(const F& handler) -> bool {
        if (m_event.type_name() == T::static_type_name()) {
            handler(static_cast<U>(m_event));
            return true;
        }

        return false;
    }

}; // class EventListener

} // namespace sap::core::events

#endif // SAPLING_ENGINE_EVENT_DISPATCHER_H
