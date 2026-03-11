cmake_minimum_required(VERSION 3.31)

#project(core-resources LANGUAGES CXX)

# ---- module: resources ----
message("Compiling module: [resources] for [core]")

add_library(core_resources)
target_compile_features(core_resources PUBLIC cxx_std_20)

target_sources(
    core_resources
    PRIVATE
        #"${CMAKE_CURRENT_SOURCE_DIR}/base_resources.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/resource_handle.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/resource_manager.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/shader_resource.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/mesh_resource.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/texture_resource.cxx"
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/"
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources.h"
    PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/"
        FILES
            #"${CMAKE_CURRENT_SOURCE_DIR}/base_resources.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/resource_handle.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/resource_manager.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/shader_resource.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/mesh_resource.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/texture_resource.h"
)

