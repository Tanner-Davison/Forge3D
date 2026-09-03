#pragma once
#include "queueFamilies.hpp"
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

/*You will only call these Next Functions once you have called getSwapChainSupportDetails*/
VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats);

VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes);

VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);

VkSwapchainKHR createSwapchain(GLFWwindow*        pWindow,
                               VkDevice           pLogicalDevice,
                               SwapchainSupport&  swapchainSupport,
                               VkSurfaceKHR       surface,
                               QueueFamilyIndices indices);
