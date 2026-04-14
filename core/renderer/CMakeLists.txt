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
)

