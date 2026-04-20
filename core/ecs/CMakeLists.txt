cmake_minimum_required(VERSION 3.31)

# ---- module: ecs ----
message("Compiling module: [ecs] for [core]")

add_library(sapling-core-ecs SHARED)
target_compile_features(sapling-core-ecs PUBLIC cxx_std_20)

target_source(
    sapling-core-ecs
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/entity.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/components/mesh_component.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/components/transform_component.cxx"
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_SOURCE_DIR}"
        FILES 
            "${CMAKE_CURRENT_SOURCE_DIR}/component.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/entity.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/components/mesh_component.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/components/transform_component.h"
)

target_link_libraries(sapling-core-ecs 
    PUBLIC 
        sapling-core-math
        glm::glm
)