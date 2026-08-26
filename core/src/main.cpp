#include "debugCallbackVulkan.hpp"
#include "physicalDevice.hpp"
#include "vulkanInstance.hpp"
#include "windowHandling.hpp"
#include <GLFW/glfw3.h>
#include <print>
#include <vulkan/vulkan.h>

int main(int argc, char** argv) {
    GLFWwindow* window = createWindow(800, 600, "Tannery");
    if (!window) {
        return 1;
    }
    VkInstance vk_instance = createInstance("Tannery");
    if (vk_instance == VK_NULL_HANDLE) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 2;
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    populateDebugMessengerCreateInfo(debug_create_info);

    /*Set to null safe to destroy if not created -------------*/
    VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
    /*--------------------------------------------------------*/

    if (CreateDebugUtilsMessengerEXT(vk_instance, &debug_create_info, nullptr, &debug_messenger) !=
        VK_SUCCESS) {
        std::println(stderr, "Failed to set up runtime debug messenger!");
        vkDestroyInstance(vk_instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 4;
    }
    /* Get Physical Device */
    VkPhysicalDevice physical_device = getPhysicalDevice(vk_instance);
    if (physical_device == VK_NULL_HANDLE) {
        std::println(stderr, "Failed to find a suitable physical device!");
        DestroyDebugUtilsMessengerEXT(vk_instance, debug_messenger, nullptr);
        vkDestroyInstance(vk_instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 5;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    DestroyDebugUtilsMessengerEXT(vk_instance, debug_messenger, nullptr);
    vkDestroyInstance(vk_instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
