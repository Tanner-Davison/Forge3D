#include "queueFamilies.hpp"
#include <print>
#include <vector>

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    if (queueFamilies.empty()) {
        std::print(stderr, "Error: No queue families found!");
        return indices;
    }

    indices.familyCount = queueFamilyCount;
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            break;
        }
    }
    return indices;
};
