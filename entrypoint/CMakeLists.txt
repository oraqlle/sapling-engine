cmake_minimum_required(VERSION 3.31)

#project(entrypoint LANGUAGES CXX)

add_executable(entrypoint entrypoint.cxx)

target_compile_features(entrypoint PUBLIC cxx_std_20)

#target_include_directories(entry PRIVATE ${std_INCLUDE_DIR})

target_compile_definitions(
    entrypoint
    PRIVATE
        "VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS"
)

target_link_libraries(
    entrypoint
    PRIVATE
        Vulkan::sdk
        glfw3

        sapling-core-renderer
)
