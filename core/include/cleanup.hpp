#pragma once
#include "debugCallbackVulkan.hpp"
#include <GLFW/glfw3.h>

static void cleanup(GLFWwindow*              window,
                    VkInstance               instance,
                    VkDebugUtilsMessengerEXT messenger,
                    VkDevice                 logicalDevice) {
    if (messenger != VK_NULL_HANDLE) {
        DestroyDebugUtilsMessengerEXT(instance, messenger, nullptr);
    }
    if (logicalDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(logicalDevice, nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}
