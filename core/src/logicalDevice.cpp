#include "logicalDevice.hpp"
#include <print>
#include <vector>

LogicalDeviceInfo createLogicalDevice(VkPhysicalDevice pPhysicalDevice, QueueFamilyIndices familyIndices) {
    LogicalDeviceInfo info;
    // This Variable needs to stick around and unmodified until vkCreateDevice actually runs
    const std::vector<float> queuePriorities(1, 1.0f); //|

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceQueueCreateInfo createInfoQueue{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = familyIndices.graphicsFamilyIndex.value(),
        .queueCount       = 1,
        .pQueuePriorities = queuePriorities.data(),
    };

    VkDeviceCreateInfo deviceInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .queueCreateInfoCount    = 1,
        .pQueueCreateInfos       = &createInfoQueue,
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,
        .enabledExtensionCount   = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures        = &deviceFeatures,

    };

    /* Attempting to create Logical Device */
    VkResult res = vkCreateDevice(pPhysicalDevice, &deviceInfo, VK_NULL_HANDLE, &info.logicalDevice);
    if (res != VK_SUCCESS) {
        std::println("Error: Could Not Create Logical Device");
        return info;
    }

    vkGetDeviceQueue(info.logicalDevice, familyIndices.graphicsFamilyIndex.value(), 0, &info.graphicsQueue);

    return info;
};
