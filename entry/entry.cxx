#include <vulkan/vulkan_raii.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std::literals;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS = true;
#else
constexpr bool ENABLE_VALIDATION_LAYERS = false;
#endif

class HelloTriangleApp {
private:
    GLFWwindow *m_window = nullptr;
    vk::raii::Context m_context;
    vk::raii::Instance m_instance = nullptr;

    static constexpr std::array m_req_validation_layers = {"VK_LAYER_KHRONOS_validation"};
    std::vector<const char *> m_req_instance_exts;

public:
    auto run() -> void {
        init_window();
        init_vulkan();
        main_loop();
        cleanup();
    }

private:
    auto init_window() -> void {
        std::clog << "Initialising Window ...\n";

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(WIDTH, HEIGHT, "Sapling Engine", nullptr, nullptr);

        _M_gen_required_instance_extensions();
    }

    auto init_vulkan() -> void {
        std::clog << "Initialising Vulkan ...\n";

        create_instance();
    }

    auto main_loop() -> void {
        std::clog << "Entering Main Loop ...\n";

        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }

        std::clog << "Exiting Main Loop ...\n";
    }

    auto cleanup() -> void {
        std::clog << "Cleaning Up ...\n";

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    auto create_instance() -> void {
        constexpr auto app_info = vk::ApplicationInfo{
            .pApplicationName = "Hello Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Sapling Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };

        if constexpr (ENABLE_VALIDATION_LAYERS) {
            const auto layer_props = m_context.enumerateInstanceLayerProperties();
            const auto unsupported_layer_iter = std::ranges::find_if(
                m_req_validation_layers, [&layer_props](const auto layer) {
                    return std::ranges::none_of(layer_props, [&layer](const auto& prop) {
                        return strcmp(prop.layerName, layer);
                    });
                }
            );

            if (unsupported_layer_iter != m_req_validation_layers.end()) {
                throw std::runtime_error(
                    "Required layer not supported: " +
                    std::string(*unsupported_layer_iter)
                );
            }
        }

        const auto extension_props = m_context.enumerateInstanceExtensionProperties();
        const auto unsupported_prop_iter =
            std::ranges::find_if(m_req_instance_exts, [&extension_props](const auto ext) {
                return std::ranges::none_of(extension_props, [&ext](const auto& prop) {
                    return strcmp(prop.extensionName, ext);
                });
            });

        if (unsupported_prop_iter != m_req_instance_exts.end()) {
            throw std::runtime_error(
                "Required extension not supported: " + std::string(*unsupported_prop_iter)
            );
        }

        auto create_info = vk::InstanceCreateInfo{
            .flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(m_req_validation_layers.size()),
            .ppEnabledLayerNames = m_req_validation_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(m_req_instance_exts.size()),
            .ppEnabledExtensionNames = m_req_instance_exts.data(),
        };

        m_instance = vk::raii::Instance(m_context, create_info);

        if (ENABLE_VALIDATION_LAYERS) {
            const auto layer_props = m_context.enumerateInstanceLayerProperties();
            const auto unsupported_layer_iter = std::ranges::find_if(
                m_req_validation_layers, [&layer_props](const auto layer) {
                    return std::ranges::none_of(layer_props, [&layer](const auto& prop) {
                        return strcmp(prop.layerName, layer);
                    });
                }
            );

            if (unsupported_layer_iter != m_req_validation_layers.end()) {
                throw std::runtime_error(
                    "Required layer not supported: " +
                    std::string(*unsupported_layer_iter)
                );
            }
        }
    }

private: // Internal helper methods
    auto _M_gen_required_instance_extensions() -> void {
        auto glfw_ext_count = uint32_t{0};
        auto glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

        auto extensions = std::vector(glfw_extensions, glfw_extensions + glfw_ext_count);

        /* Ensure support for Vulkan on macOS (via MoltonVK)
         * as if from VulkanSDK 1.3.216 */
        extensions.push_back(vk::KHRPortabilityEnumerationExtensionName);

        m_req_instance_exts = extensions;
    }
};

auto main() -> int {

    try {
        auto app = HelloTriangleApp{};
        app.run();
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
