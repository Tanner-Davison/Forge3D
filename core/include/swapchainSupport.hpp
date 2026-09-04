#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>

struct SwapchainSupport {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentationModes;

    bool isComplete() const {
        return !formats.empty() && !presentationModes.empty();
    }
};

SwapchainSupport getSwapchainSupportDetails(VkPhysicalDevice pPhysicalDevice,
                                            VkSurfaceKHR     pSurface);
