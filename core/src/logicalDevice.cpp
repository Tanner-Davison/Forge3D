#include "logicalDevice.hpp"
#include <print>
#include <set>
#include <vector>

LogicalDeviceInfo createLogicalDevice(VkPhysicalDevice   pPhysicalDevice,
                                      QueueFamilyIndices familyIndices) {
    LogicalDeviceInfo info;
    // This Variable needs to stick around and unmodified until vkCreateDevice actually runs
    const std::vector<float> queuePriorities(1, 1.0f);

    VkPhysicalDeviceFeatures deviceFeatures{};

    std::set<uint32_t> uniqueQueueFamilies = {familyIndices.graphicsFamilyIndex.value(),
                                              familyIndices.presentFamilyIndex.value()};

    /*CREATES QUEUE INFO FOR EACH FAMILY*/
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    for (const auto family : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo createInfoQueue{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = family,
            .queueCount       = 1,
            .pQueuePriorities = queuePriorities.data(),
        };
        queueCreateInfos.emplace_back(createInfoQueue);
    }

    VkDeviceCreateInfo deviceInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos       = queueCreateInfos.data(),
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,
        .enabledExtensionCount   = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures        = &deviceFeatures,

    };

    /* CREATE LOGICAL DEVICE */
    VkResult res =
        vkCreateDevice(pPhysicalDevice, &deviceInfo, VK_NULL_HANDLE, &info.logicalDevice);
    if (res != VK_SUCCESS) {
        std::println("Error: Could Not Create Logical Device");
        return info;
    }

    vkGetDeviceQueue(info.logicalDevice,
                     familyIndices.graphicsFamilyIndex.value(),
                     0,
                     &info.graphicsQueue);
    vkGetDeviceQueue(info.logicalDevice,
                     familyIndices.presentFamilyIndex.value(),
                     0,
                     &info.presentQueue);

    return info;
};
