#include "debugCallback.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <print>
#include <vector>
#include <vulkan/vulkan.h>

/*GLFW CALLBACKS */
void error_callback(int error_code, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error_code, description);
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char** argv) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::print("GLFW Init Failed");
        return 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Tannery", NULL, NULL);

    if (!window) {
        std::print("GLFW Window Creation Failed");
        glfwTerminate();
        return 2;
    }

    glfwSetKeyCallback(window, key_callback);

    /* VULKAN STRUCT INFO & INSTANCE CREATION*/
    VkApplicationInfo app_info = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = "Tannery",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = "Tannery",
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
        return 3;
    }

    VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE; /*Set to null safe to destroy if not
                                                                created*/
    if (CreateDebugUtilsMessengerEXT(vk_instance, &debug_create_info, nullptr, &debug_messenger) !=
        VK_SUCCESS) {
        std::println(stderr, "Failed to set up runtime debug messenger!");
        return 4;
    }
    /*POLL EVENTS*/
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    DestroyDebugUtilsMessengerEXT(vk_instance, debug_messenger, nullptr);
    vkDestroyInstance(vk_instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
