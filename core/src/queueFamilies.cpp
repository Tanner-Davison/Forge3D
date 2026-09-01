#include "queueFamilies.hpp"
#include <print>
#include <vector>

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pPhysicalDevice, VkSurfaceKHR pSurface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice,
                                             &queueFamilyCount,
                                             queueFamilies.data());

    if (queueFamilies.empty()) {
        std::println(stderr, "Error: No queue families found!");
        return indices;
    }

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            if (!indices.graphicsFamilyIndex.has_value()) {
                indices.graphicsFamilyIndex = i;
            }
        }
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(pPhysicalDevice, i, pSurface, &presentSupport);
        if (presentSupport) {
            if (!indices.presentFamilyIndex.has_value()) {
                indices.presentFamilyIndex = i;
            }
        }
        if (indices.isComplete()) {
            break;
        }
    }

    std::println("------Family Queue Info-------");
    if (indices.graphicsFamilyIndex.has_value()) {
        std::println("Graphics Family Index: {}", indices.graphicsFamilyIndex.value());
    } else {
        std::println("Graphics Family Index: not found");
    }
    if (indices.presentFamilyIndex.has_value()) {
        std::println("Present Family Index: {}", indices.presentFamilyIndex.value());
    } else {
        std::println("Present Family Index: Not Found");
    }
    std::println("-------------------------------");

    return indices;
};
