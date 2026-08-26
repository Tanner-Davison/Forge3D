#include "debugCallbackVulkan.hpp"
#include "physicalDevice.hpp"
#include "queueFamilies.hpp"
#include "vulkanInstance.hpp"
#include "windowHandling.hpp"
#include <GLFW/glfw3.h>
#include <print>
#include <vulkan/vulkan.h>

void cleanup(GLFWwindow* window, VkInstance instance, VkDebugUtilsMessengerEXT messenger) {
    if (messenger != VK_NULL_HANDLE) {
        DestroyDebugUtilsMessengerEXT(instance, messenger, nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

int main(int argc, char** argv) {
    GLFWwindow* window = createWindow(800, 600, "Tannery");
    if (!window) {
        return 1;
    }
    VkInstance vkInstance = createInstance("Tannery");
    if (vkInstance == VK_NULL_HANDLE) {
        cleanup(window, VK_NULL_HANDLE, VK_NULL_HANDLE);
        return 2;
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    populateDebugMessengerCreateInfo(debug_create_info);

    /*Create debug messenger*/
    VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

    if (CreateDebugUtilsMessengerEXT(vkInstance, &debug_create_info, nullptr, &debug_messenger) !=
        VK_SUCCESS) {
        std::println(stderr, "Failed to set up runtime debug messenger!");
        cleanup(window, vkInstance, VK_NULL_HANDLE);
        return 3;
    }

    /* Get Physical Device */
    VkPhysicalDevice physicalDevice = getPhysicalDevice(vkInstance);

    if (physicalDevice == VK_NULL_HANDLE) {
        std::println(stderr, "Failed to find a suitable physical device!");
        cleanup(window, vkInstance, debug_messenger);
        return 4;
    }

    printPhysicalDevices(vkInstance);
    QueueFamilyIndices graphicsFamilyQueue = findQueueFamilies(physicalDevice);
    if (!graphicsFamilyQueue.graphicsFamily.has_value()) {
        std::println(stderr, "Error: No graphics family queue found!");
        cleanup(window, vkInstance, debug_messenger);
        return 5;
    }
    std::println("Family Queue Information:");
    std::println("Queue Family Count: {}", graphicsFamilyQueue.familyCount);
    std::println("Graphics Family Index: {}", graphicsFamilyQueue.graphicsFamily.value());

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    cleanup(window, vkInstance, debug_messenger);

    return 0;
}
