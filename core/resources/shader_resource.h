// <core/resources/shader_resource.h> -*- C++ -*-

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

#ifndef SAPLING_ENGINE_SHADER_RESOURCE_H
#define SAPLING_ENGINE_SHADER_RESOURCE_H

#include <base_resource.h>
#include <resource_handle.h>
#include <resource_manager.h>

#include "vulkan/vulkan.hpp"

#include <string>
#include <vector>

namespace sap::core::resources {

class Shader : public Resource {
private:
    vk::ShaderModule m_shader_module;
    vk::ShaderStageFlagBits m_stage;

public:
    explicit Shader(const std::string& id, vk::ShaderStageFlagBits shader_stage);

    ~Shader() override;

    auto shader_module() -> vk::ShaderModule;

    auto stage() -> vk::ShaderStageFlagBits;

protected:
    bool do_load() override;

    bool do_unload() override;

private:
    auto _M_read_file(const std::string& file_path, std::vector<char> shader_src) -> bool;

    auto _M_create_shader_module(const std::vector<char>& code) -> void;

    auto _M_device() -> vk::Device;
}; // class Shader

} // namespace sap::core::resources

#endif // SAPLING_ENGINE_SHADER_RESOURCE_H
