#include "cleanup.hpp"
#include "debugCallbackVulkan.hpp"
#include "logicalDevice.hpp"
#include "physicalDevice.hpp"
#include "queueFamilies.hpp"
#include "surface.hpp"
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

    VkInstance instance = createInstance("Tannery");

    if (instance == VK_NULL_HANDLE) {
        cleanup(window, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE);
        return 2;
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    populateDebugMessengerCreateInfo(debug_create_info);

    /*CREATE DEBUG MESSENGER*/
    VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

    if (CreateDebugUtilsMessengerEXT(instance,
                                     &debug_create_info,
                                     nullptr,
                                     &debug_messenger) != VK_SUCCESS) {
        std::println(stderr, "Failed to set up runtime debug messenger!");
        cleanup(window, instance, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE);
        return 3;
    }

    /* GET PHYSICAL DEVICE */
    VkPhysicalDevice physicalDevice = getPhysicalDevice(instance);

    if (physicalDevice == VK_NULL_HANDLE) {
        std::println(stderr, "Failed to find a suitable physical device!");
        cleanup(window, instance, debug_messenger, VK_NULL_HANDLE, VK_NULL_HANDLE);
        return 4;
    }
    /* Uncomment to view all Physical Devices found--------------------------------
     * printPhysicalDevices(instance); */

    /*CREATE WINDOW SURFACE*/
    VkSurfaceKHR surface = getWindowSurface(instance, window);

    if (surface == VK_NULL_HANDLE) {
        std::println(stderr,
                     "Error: Surface creation return a VK_NULL_HANDLE creation failed.");
        cleanup(window, instance, debug_messenger, VK_NULL_HANDLE, VK_NULL_HANDLE);
        return 5;
    }

    /*QUEUE FAMILY INDICES*/
    QueueFamilyIndices graphicsFamilyQueue = findQueueFamilies(physicalDevice, surface);

    if (!graphicsFamilyQueue.isComplete()) {
        std::println(stderr, "Error: Missing Graphics Family or Present family index!");
        cleanup(window, instance, debug_messenger, VK_NULL_HANDLE, VK_NULL_HANDLE);
        return 6;
    }

    /*LOGICAL DEVICE CREATION*/
    LogicalDeviceInfo logicalDeviceInfo(
        createLogicalDevice(physicalDevice, graphicsFamilyQueue));

    if (logicalDeviceInfo.logicalDevice == VK_NULL_HANDLE) {
        std::println(stderr, "Error: Failed to create logical device");
        cleanup(window,
                instance,
                debug_messenger,
                logicalDeviceInfo.logicalDevice,
                VK_NULL_HANDLE);
        return 7;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    cleanup(window,
            instance,
            debug_messenger,
            logicalDeviceInfo.logicalDevice,
            VK_NULL_HANDLE);

    return 0;
}
