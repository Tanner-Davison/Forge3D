#pragma once
#include <vector>
#include <vulkan/vulkan.h>

struct SwapchainSupport {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;

    bool isComplete() const {
        return !formats.empty() && !presentModes.empty();
    }
};

SwapchainSupport getSwapchainSupportDetails(VkPhysicalDevice pPhysicalDevice,
                                            VkSurfaceKHR     pSurface);
