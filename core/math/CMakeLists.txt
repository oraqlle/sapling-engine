cmake_minimum_required(VERSION 3.31)

# ---- module: math ----
message("Compiling module: [math] for [core]")

add_library(sapling-core-math SHARED)
target_compile_features(sapling-core-math PUBLIC cxx_std_20)

target_source(
    sapling-core-math
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/bounding_box.cxx"
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_SOURCE_DIR}"
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/bounding_box.h"
)

target_link_libraries(sapling-core-math PUBLIC glm::glm)