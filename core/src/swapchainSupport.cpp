#include "swapchainSupport.hpp"
#include "vulkan_core.h"
#include <print>

/*
 *   Checks for Capabilites ->  Surface  formats -> Present Modes
 *   && returns all in swapchainDetails
 *
 */

SwapchainSupportDetails getSwapchainSupportDetails(VkPhysicalDevice pPhysicalDevice,
                                                   VkSurfaceKHR     pSurface) {
    SwapchainSupportDetails swapchainDetails;

    /*CAPABILITIES*/
    VkResult capabilitiesRes =
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pPhysicalDevice,
                                                  pSurface,
                                                  &swapchainDetails.capabilities);
    if (capabilitiesRes != VK_SUCCESS) {
        std::println(stderr, "Error: Could not see Device Surface Capabilities.");
    }

    /*SURFACE FORMATS*/
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pPhysicalDevice, pSurface, &formatCount, nullptr);
    swapchainDetails.formats.resize(formatCount);
    VkResult formatRes = vkGetPhysicalDeviceSurfaceFormatsKHR(pPhysicalDevice,
                                                              pSurface,
                                                              &formatCount,
                                                              swapchainDetails.formats.data());
    if (formatRes != VK_SUCCESS) {
        std::println(stderr, "Error: Failed to get physical device surface format KHR");
    }

    /*PRESENT MODE*/
    uint32_t presentModeCount = 0;
    VkResult presentRes       = vkGetPhysicalDeviceSurfacePresentModesKHR(pPhysicalDevice,
                                                                    pSurface,
                                                                    &presentModeCount,
                                                                    nullptr);
    swapchainDetails.presentModes.resize(presentModeCount);
    if (presentRes != VK_SUCCESS) {
        std::println(stderr, "Error: Could not find present modes count");
    }

    if (!swapchainDetails.isComplete()) {
        std::println(stderr, "Error: swapchain details failed to find any of the following 2");
        std::println(stderr, "formats");
        std::println(stderr, "presentModes");
        return swapchainDetails;
    }
    return swapchainDetails;
};
