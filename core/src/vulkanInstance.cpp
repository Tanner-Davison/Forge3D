#include "vulkanInstance.hpp"
#include "debugCallbackVulkan.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <print>
#include <vector>

VkInstance createInstance(const char* appName) {
    VkApplicationInfo app_info = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = appName,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = appName,
        .engineVersion      = 1,
        .apiVersion         = VK_API_VERSION_1_3,
    };

    /*GATHER ING REQUIRED GLFW EXTENSIONS*/
    uint32_t     count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    /*Core Extensions list initialized with debug utils extension*/
    std::vector<const char*> extensions_with_debug_utils = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

    if (extensions != NULL) {
        for (uint32_t i = 0; i < count; i++) {
            extensions_with_debug_utils.push_back(extensions[i]);
        }
    } else {
        std::print(stderr, "Failed to retrieve extensions.\n");
    }

    /*Mac Compatibility configuration*/
    VkInstanceCreateFlags instance_flags = 0;
#ifdef __APPLE__
    extensions_with_debug_utils.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    instance_flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    /*Print Active Extensions*/
    std::print("Active Extensions:\n");
    for (const auto& ext : extensions_with_debug_utils) {
        std::print("  {}\n", ext);
    }
    std::print("\n");

    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};

    /*Setup debug messenger options to catch early instance creation errors*/
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    populateDebugMessengerCreateInfo(debug_create_info);

    VkInstanceCreateInfo instance_info = {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = &debug_create_info, /*Plugs messenger into early instance*/
        .flags                   = instance_flags,
        .pApplicationInfo        = &app_info,
        .enabledLayerCount       = std::size(layers),
        .ppEnabledLayerNames     = layers,
        .enabledExtensionCount   = static_cast<uint32_t>(extensions_with_debug_utils.size()),
        .ppEnabledExtensionNames = extensions_with_debug_utils.data(),
    };

    VkInstance vk_instance;

    VkResult res = vkCreateInstance(&instance_info, nullptr, &vk_instance);
    if (res != VK_SUCCESS) {
        std::print(stderr, "Failed to create instance");
        return VK_NULL_HANDLE;
    }
    return vk_instance;
};
