#include "logicalDevice.hpp"
#include <cstring>
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

    /*CHECK FOR EXTENSIONS COMPATIBILITY ON PHYSICAL DEVICE*/
    uint32_t extensionCount = 0;
    VkResult extensionRes   = vkEnumerateDeviceExtensionProperties(pPhysicalDevice,
                                                                 nullptr,
                                                                 &extensionCount,
                                                                 nullptr);
    if (extensionRes != VK_SUCCESS) {
        std::println(stderr, "Error: Unable to enumerate device extension properties");
        return info;
    }
    /*EXTENSIONS VECTOR*/
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(pPhysicalDevice,
                                         nullptr,
                                         &extensionCount,
                                         extensions.data());

    bool driverPropertiesSupported   = false;
    bool swapchainExtensionSupported = false;
    bool canStop = driverPropertiesSupported && swapchainExtensionSupported;

    /*Checking for support of VK_KHR_bind_memory2*/
    for (uint32_t i = 0; i < extensionCount; i++) {
        if (!driverPropertiesSupported) {
            if (strcmp(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME, extensions[i].extensionName) ==
                0) {
                driverPropertiesSupported =
                    true; // DRIVER_PROPERTIES_EXTENSION_NAME is supported
            };
        }
        if (!swapchainExtensionSupported) {
            if (strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, extensions[i].extensionName) == 0) {
                swapchainExtensionSupported = true; // SWAPCHAIN_EXTENSION_NAME is supported
            }
        }
        canStop = driverPropertiesSupported && swapchainExtensionSupported;
        if (canStop) {
            break;
        }
    };

    std::vector<const char*> deviceExtensions;
    if (driverPropertiesSupported) {
        deviceExtensions.push_back(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME);
    }
    if (swapchainExtensionSupported) {
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    VkDeviceCreateInfo deviceInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos       = queueCreateInfos.data(),
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,
        .enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
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
