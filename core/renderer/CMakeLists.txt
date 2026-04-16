cmake_minimum_required(VERSION 3.31)

#project(core-resources LANGUAGES CXX)

# ---- module: resources ----
message("Compiling module: [renderer] for [core]")

add_library(sapling-core-renderer SHARED)
target_compile_features(sapling-core-renderer PUBLIC cxx_std_20)

target_sources(
    sapling-core-renderer
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/culling_system.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/geometry_render_pass.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/lighting_render_pass.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/post_processing_render_pass.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/render_graph.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/render_pass.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/render_pass_manager.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer_pipeline.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/render_target.cxx"
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/"
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/renderer.h"
    PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/"
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/culling_system.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/geometry_render_pass.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/lighting_render_pass.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/post_processing_render_pass.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/render_graph.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/render_pass.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/render_pass_manager.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/renderer_pipeline.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/render_target.h"
)

