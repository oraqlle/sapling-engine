// <core/resources/hot_reload_resource_manager.cxx> -*- C++ -*-

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

#include "hot_reload_resource_manager.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

namespace sap::core::resources {

HotReloadResourceManager::HotReloadResourceManager() { _M_start_watcher(); }

HotReloadResourceManager::~HotReloadResourceManager() { _M_stop_watcher(); }

auto HotReloadResourceManager::_M_start_watcher() -> void {
    m_running = true;
    m_watcher_thread = std::thread([this]() { _M_watcher(); });
}

auto HotReloadResourceManager::_M_stop_watcher() -> void {
    m_running = false;

    if (m_watcher_thread.joinable()) {
        m_watcher_thread.join();
    }
}

auto HotReloadResourceManager::_M_watcher() -> void {
    while (m_running) {
        for (auto& [file_path, timestamp] : m_file_timestamps) {
            try {
                auto current_timestamp = fs::last_write_time(file_path);

                if (current_timestamp != timestamp) {
                    _M_reload_resource(file_path);
                    timestamp = current_timestamp;
                }
            } catch (const fs::filesystem_error& err) {
                std::clog << std::format(
                    "File at path: '{}' does not exist or cannot be accessed", file_path
                );
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

auto HotReloadResourceManager::_M_reload_resource(const std::string& file_path) -> void {}

} // namespace sap::core::resources
