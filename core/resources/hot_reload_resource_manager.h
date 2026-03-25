// <core/resources/hot_reload_resource_manager.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_HOT_RELOAD_RESOURCE_MANAGER_H
#define SAPLING_ENGINE_HOT_RELOAD_RESOURCE_MANAGER_H

#include "resource_manager.h"

#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

namespace sap::core::resources {

class HotReloadResourceManager : public ResourceManager {
private:
    std::unordered_map<std::string, std::filesystem::file_time_type> m_file_timestamps;
    std::thread m_watcher_thread;
    bool m_running = false;

public:
    HotReloadResourceManager();

    ~HotReloadResourceManager();

    auto _M_start_watcher() -> void;

    auto _M_stop_watcher() -> void;

    template <typename T>
    auto load(const std::string& resource_id) -> ResourceHandle<T> {
        auto handle = ResourceManager::load<T>(resource_id);

        auto file_path = _M_file_path<T>(resource_id);

        try {
            m_file_timestamps[file_path] = std::filesystem::last_write_time(file_path);
        } catch (const std::filesystem::filesystem_error& err) {
            std::clog << std::format(
                "File at path: '{}' does not exist or cannot be accessed", file_path
            );
        }
    }

private:
    template <typename T>
    auto _M_file_path(const std::string& resource_id) -> std::string {}

    auto _M_watcher() -> void;

    auto _M_reload_resource(const std::string& file_path) -> void;

}; // class HotReloadResourceManager

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_HOT_RELOAD_RESOURCE_MANAGER_H
