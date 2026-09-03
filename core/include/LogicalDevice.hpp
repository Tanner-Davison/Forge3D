#pragma once
#include "queueFamilies.hpp"
#include <vulkan/vulkan.h>

class LogicalDevice {
  public:
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue  graphicsQueue = VK_NULL_HANDLE;
    VkQueue  presentQueue  = VK_NULL_HANDLE;

    LogicalDevice(VkPhysicalDevice pPhysicalDevice, const QueueFamilyIndices& indices);

    ~LogicalDevice();
};
