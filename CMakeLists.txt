cmake_minimum_required(VERSION 3.31)

project(
    sapling_engine
    VERSION 0.1.0
    DESCRIPTION "simple 3D game engine"
    LANGUAGES CXX C
)

# ---- Global Defines ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Options ----


# ---- Dependencies ----
include(FetchContent)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG f1c79c02822848a9bed4315b12c8c8f3761e1296
)
FetchContent_MakeAvailable(stb)

# ---- Find Packages ----
find_package(glfw3 3.4 REQUIRED)
find_package(glm 1.0.1 REQUIRED)
find_package(Vulkan 1.4.341 REQUIRED)
find_package(tinyobjloader 1.0.4 REQUIRED)

# ---- Find Programs ----


# ---- Vulkan SDK Target ----
add_library(VulkanSDK INTERFACE)
add_library(Vulkan::sdk ALIAS VulkanSDK)
target_link_libraries(VulkanSDK INTERFACE Vulkan::Vulkan)
target_compile_definitions(VulkanSDK
      INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

# ---- Engine Library ----


# ---- Entry Point Bootstrapping ----
add_executable(entry entry/entry.cxx)
set_target_properties(entry PROPERTIES CXX_STANDARD 20)
target_include_directories(entry PRIVATE ${STD_INCLUDEDIR})
target_link_libraries(entry PRIVATE Vulkan::sdk glfw3 stb)

target_compile_definitions(entry PRIVATE "VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS")

# ---- Installation ----
install(TARGETS entry RUNTIME COMPONENT game_runtime)
include(CPack)
