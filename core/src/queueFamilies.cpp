#include "queueFamilies.hpp"
#include <print>
#include <vector>

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pPhysicalDevice) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    if (queueFamilies.empty()) {
        std::println(stderr, "Error: No queue families found!");
        return indices;
    }

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamilyIndex = i;
            break;
        }
    }

    std::println("------Family Queue Info-------");
    if (indices.graphicsFamilyIndex.has_value()) {
        std::println("Graphics Family Index: {}", indices.graphicsFamilyIndex.value());
    } else {
        std::println("Graphics Family Index: not found");
    }
    std::println("-------------------------------");

    return indices;
};
