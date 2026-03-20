// <core/resources/shader_resource.cxx> -*- C++ -*-

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

#include <shader_resource.h>

#include "vulkan/vulkan.hpp"

#include <format>
#include <string>
#include <vector>

using namespace std::literals;

namespace sap::core::resources {

Shader::Shader(const std::string& id, vk::ShaderStageFlagBits stage)
    : Resource(id)
    , m_stage(stage) {}

Shader::~Shader() { unload(); }

auto Shader::do_load() -> bool {
    auto ext = ""s;

    switch (m_stage) {
        case vk::ShaderStageFlagBits::eVertex:
            ext = ".vert";
            break;
        case vk::ShaderStageFlagBits::eFragment:
            ext = ".frag";
            break;
        case vk::ShaderStageFlagBits::eCompute:
            ext = ".comp";
            break;
        default:
            return false;
    }

    auto file_path = std::format("shaders/{}.{}", get_id(), ext);

    auto shader_src = std::vector<char>{};
    if (!_M_read_file(file_path, shader_src)) {
        return false;
    }

    _M_create_shader_module(shader_src);

    return true;
}

auto Shader::do_unload() -> bool {
    if (is_loaded()) {

        auto device = _M_device();

        device.destroyShaderModule(m_shader_module);

        return true;
    }

    return false;
}

auto Shader::shader_module() -> vk::ShaderModule { return m_shader_module; }

auto Shader::stage() -> vk::ShaderStageFlagBits { return m_stage; }

auto Shader::_M_read_file(const std::string& file_path, std::vector<char> shader_src)
    -> bool {
    return true;
}

auto Shader::_M_create_shader_module(const std::vector<char>& shader_src) -> void {}

auto Shader::_M_device() -> vk::Device { return vk::Device(); }

} // namespace sap::core::resources
