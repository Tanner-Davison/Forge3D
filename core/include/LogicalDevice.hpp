#pragma once
#include "queueFamilies.hpp"
#include <vulkan/vulkan.h>

class LogicalDevice {
  public:
    LogicalDevice(VkPhysicalDevice pPhysicalDevice, const QueueFamilyIndices& indices);

    // copy constructors
    LogicalDevice(const LogicalDevice&)            = delete;
    LogicalDevice& operator=(const LogicalDevice&) = delete;
    // move constructors
    LogicalDevice(LogicalDevice&&)            = delete;
    LogicalDevice& operator=(LogicalDevice&&) = delete;

    VkDevice handle() const;
    VkQueue  GraphicsQueueHandle() const;
    VkQueue  PresentQueueHandle() const;
    ~LogicalDevice();

  private:
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue  graphicsQueue = VK_NULL_HANDLE;
    VkQueue  presentQueue  = VK_NULL_HANDLE;
};
