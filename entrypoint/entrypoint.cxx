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
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

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
    vk::raii::PhysicalDevice m_physical_device = nullptr;

    static constexpr std::array m_req_validation_layers = {"VK_LAYER_KHRONOS_validation"};
    static constexpr std::array m_req_devices_exts = {vk::KHRSwapchainExtensionName};
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

        m_req_instance_exts = gen_required_instance_extensions();
    }

    static auto gen_required_instance_extensions() -> std::vector<const char *> {
        auto glfw_ext_count = uint32_t{0};
        auto glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

        auto extensions = std::vector(glfw_extensions, glfw_extensions + glfw_ext_count);

        /* Ensure support for Vulkan on macOS (via MoltonVK)
         * as if from VulkanSDK 1.3.216 */
        extensions.push_back(vk::KHRPortabilityEnumerationExtensionName);

        return extensions;
    }

    auto init_vulkan() -> void {
        std::clog << "Initialising Vulkan ...\n";

        create_instance();
        pick_physical_device();
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
                    return strcmp(prop.extensionName, ext) == 0;
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
                        return strcmp(prop.layerName, layer) == 0;
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

    auto pick_physical_device() -> void {
        auto devices = m_instance.enumeratePhysicalDevices();

        if (devices.empty()) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        auto candidates = std::multimap<int32_t, vk::raii::PhysicalDevice>{};

        for (const auto& device : devices) {
            auto score = device_suitablility_score(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (const auto& opt = candidates.rbegin(); opt->first > 0) {
            m_physical_device = std::move(opt->second);
        } else {
            throw std::runtime_error("Failed to find suitable GPU!");
        }
    }

    auto device_suitablility_score(const vk::raii::PhysicalDevice& device) const
        -> int32_t {
        const auto device_props = m_physical_device.getProperties();
        const auto device_feats = m_physical_device.getFeatures();
        const auto device_exts = device.enumerateDeviceExtensionProperties();
        auto score = int32_t{0};

        if (device_props.apiVersion >= vk::ApiVersion13) {
            score += 1000;
        } else if (device_props.apiVersion >= vk::ApiVersion14) {
            score += 2000;
        } else {
            score -= 1000;
        }

        if (device_props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
            score += 1000;
        }

        score += device_props.limits.maxImageDimension2D;

        auto queue_families_props = device.getQueueFamilyProperties();
        if (std::ranges::any_of(queue_families_props, [](const auto& qfprops) {
                return !!(qfprops.queueFlags & vk::QueueFlagBits::eGraphics);
            })) {
            score += 1000;
        } else {
            score -= 1000;
        }

        for (const auto req_ext : m_req_devices_exts) {
            if (std::ranges::any_of(device_exts, [&req_ext](const auto& ext) {
                    return strcmp(req_ext, ext.extensionName) == 0;
                })) {
                score += 1000;
            } else {
                score -= 1000;
            }
        }

        if (device_feats.geometryShader) {
            score += 1000;
        } else {
            score -= 1000;
        }

        auto features2 = device.template getFeatures2<
            vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

        if (features2.template get<vk::PhysicalDeviceVulkan13Features>()
                .dynamicRendering) {
            score += 1000;
        } else {
            score -= 1000;
        }

        if (features2.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                .extendedDynamicState) {
            score += 1000;
        } else {
            score -= 1000;
        };

        return score;
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
