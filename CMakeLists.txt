cmake_minimum_required(VERSION 3.31)

project(
    sapling_engine
    VERSION 0.1.0
    DESCRIPTION "simple 3D game engine"
    LANGUAGES CXX C
)

# ---- Global Defines ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Options ----


# ---- Dependencies ----
include(FetchContent)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG f1c79c02822848a9bed4315b12c8c8f3761e1296
)
FetchContent_MakeAvailable(stb)
FetchContent_GetProperties(stb)

if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
    message(STATUS "stb_image.h not populated.")
endif()

set(std_INCLUDE_DIR ${stb_SOURCE_DIR})

mark_as_advanced(stb_INCLUDE_DIR)

# ---- Find Packages ----
find_package(glfw3 3.4 REQUIRED)
find_package(glm 1.0.1 REQUIRED)
find_package(Vulkan 1.4.341 REQUIRED)
find_package(tinyobjloader 2.0.0 REQUIRED)

# ---- Vulkan SDK Target ----
add_library(VulkanSDK INTERFACE)
add_library(Vulkan::sdk ALIAS VulkanSDK)
target_link_libraries(VulkanSDK INTERFACE Vulkan::Vulkan)
target_compile_definitions(VulkanSDK
      INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

# ---- Engine Library ----
add_subdirectory(core)

# ---- Entry Point Bootstrapping ----
add_subdirectory(entrypoint)

# ---- Installation ----
#install(TARGETS entry RUNTIME COMPONENT game_runtime)
include(CPack)
