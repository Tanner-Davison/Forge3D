#pragma once
#include <vulkan/vulkan.h>

class VulkanInstance {
  public:
    VulkanInstance(const char* appName);

    ~VulkanInstance();

    // copy constructors
    VulkanInstance(const VulkanInstance&)            = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

    // move constructors
    VulkanInstance(VulkanInstance&&)            = delete;
    VulkanInstance& operator=(VulkanInstance&&) = delete;

    VkInstance handle() const;

  private:
    VkInstance               instance{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
};
