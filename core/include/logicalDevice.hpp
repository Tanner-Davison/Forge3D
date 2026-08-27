#pragma once
#include "queueFamilies.hpp"
#include <vulkan/vulkan.h>

struct LogicalDeviceInfo {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue  graphicsQueue = VK_NULL_HANDLE;
    VkQueue  presentQueue  = VK_NULL_HANDLE;
};

LogicalDeviceInfo createLogicalDevice(VkPhysicalDevice pPhysicalDevice, QueueFamilyIndices familyIndices);
