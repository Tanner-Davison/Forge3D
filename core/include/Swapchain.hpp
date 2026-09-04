#pragma once

#include "queueFamilies.hpp"
#include "swapchainSupport.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class Swapchain {
  public:
    // Constructor
    Swapchain(VkDevice                  pLogicalDevice,
              VkSurfaceKHR              surface,
              const SwapchainSupport&   support,
              GLFWwindow*               window,
              const QueueFamilyIndices& indices);

    // Copy && move constructors deletion
    Swapchain(const Swapchain&)            = delete;
    Swapchain& operator=(const Swapchain&) = delete;
    Swapchain(Swapchain&&)                 = delete;
    Swapchain& operator=(Swapchain&&)      = delete;

    // Destructor
    ~Swapchain();

    // Handle
    VkSwapchainKHR handle() const;

  private:
    std::vector<VkImageView> imageView; // will use/implement later;
    VkDevice                 LogicalDeviceHandle = VK_NULL_HANDLE;
    VkSwapchainKHR           swapchain           = VK_NULL_HANDLE;
    VkSurfaceFormatKHR       chooseSwapSurfaceFormat(const SwapchainSupport& support);
    VkPresentModeKHR         chooseSwapPresentMode(const SwapchainSupport& support);
    VkExtent2D               chooseSwapExtent(GLFWwindow*                     window,
                                              const VkSurfaceCapabilitiesKHR& capabilities);
};
