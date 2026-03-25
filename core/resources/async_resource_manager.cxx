// <core/resources/async_resource_manager.cxx> -*- C++ -*-

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

#include "async_resource_manager.h"
#include <mutex>
#include <utility>

namespace sap::core::resources {

AsyncResourceManager::AsyncResourceManager() {
    _M_start_worker();
}

AsyncResourceManager::~AsyncResourceManager() {
    _M_stop_worker();
}

auto AsyncResourceManager::_M_start_worker() -> void {
    m_running = true;
    m_worker = std::thread([this]() {
        _M_job();
    });
}

auto AsyncResourceManager::_M_stop_worker() -> void {
    {
        auto lock = std::lock_guard<std::mutex>(m_queue_mutex);
        m_running = false;
    }

    m_condition.notify_one();

    if (m_worker.joinable()) {
        m_worker.join();
    }
}

auto AsyncResourceManager::_M_job() -> void {
    while (m_running) {
        auto task = std::function<void()>();

        {
            auto ulock = std::unique_lock<std::mutex>(m_queue_mutex);
            m_condition.wait(ulock, [this]() {
                return !m_tasks.empty() || !m_running;
            });

            if (!m_running && m_tasks.empty()) {
                return;
            }

            task = std::move(m_tasks.front());
            m_tasks.pop();
        }

        task();
    }
}

} // namespace sap::core::resources
