#pragma once
#include <vector>
#include <vulkan/vulkan.h>

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;

    bool isComplete() const {
        return !formats.empty() && !presentModes.empty();
    }
};

SwapchainSupportDetails getSwapchainSupportDetails(VkPhysicalDevice pPhysicalDevice, VkSurfaceKHR pSurface);
