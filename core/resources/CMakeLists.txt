cmake_minimum_required(VERSION 3.31)

# ---- module: resources ----
message("Compiling module: resources")

add_library(core::resources)
target_compile_features(core::resources cxx_std_20)

target_sources(
    core::resources
    PRIVATE
        #"${CMAKE_CURRENT_SOURCE_DIR}/resources/base_resources.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/resources/resource_manager.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/resources/shader_resource.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/resources/mesh_resource.cxx"
        #"${CMAKE_CURRENT_SOURCE_DIR}/resources/texture_resource.cxx"
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.h"
    PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
        FILES
            #"${CMAKE_CURRENT_SOURCE_DIR}/resources/base_resources.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/resources/resource_handle.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/resources/resource_manager.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/resources/shader_resource.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/resources/mesh_resource.h"
            #"${CMAKE_CURRENT_SOURCE_DIR}/resources/texture_resource.h"
)

