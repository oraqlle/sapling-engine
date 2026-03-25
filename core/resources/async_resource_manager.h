// <core/resources/async_resource_manager.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_ASYNC_RESOURCE_MANAGER_H
#define SAPLING_ENGINE_ASYNC_RESOURCE_MANAGER_H

#include "resource_handle.h"
#include "resource_manager.h"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace sap::core::resources {

class AsyncResourceManager {
private:
    ResourceManager m_resource_manager;
    std::thread m_worker;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    bool m_running = false;

public:
    AsyncResourceManager();

    ~AsyncResourceManager();

    template <typename T>
    auto load_async(
        const std::string& resource_id,
        std::function<void(ResourceHandle<T>)> callback
    ) -> void {
        auto lock = std::lock_guard<std::mutex>(m_queue_mutex);

        m_tasks.push([this, resource_id, callback]() {
            auto handle = m_resource_manager.load<T>(resource_id);
            callback(handle);
        });

        m_condition.notify_one();
    }

private:
    auto _M_start_worker() -> void;

    auto _M_stop_worker() -> void;

    auto _M_job() -> void;

}; // class AsyncResourceManager

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_ASYNC_RESOURCE_MANAGER_H
