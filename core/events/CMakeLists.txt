cmake_minimum_required(VERSION 3.31)

#project(core-resources LANGUAGES CXX)

# ---- module: resources ----
message("Compiling module: [events] for [core]")

add_library(sapling-core-events SHARED)
target_compile_features(sapling-core-events PUBLIC cxx_std_20)

target_sources(
    sapling-core-events
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/event_bus.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/keypress_event.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/window_resize_event.cxx"
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/"
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/events.h"
    PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/"
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/base_event.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/event_bus.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/event_dispatcher.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/event_listener.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/keypress_event.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/window_resize_event.h"
)

