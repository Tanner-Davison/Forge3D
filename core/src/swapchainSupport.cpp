#include "swapchainSupport.hpp"
#include <print>

/*
 *   Checks for Capabilites ->  Surface  formats -> Present Modes
 *   && returns all in swapchainDetails
 *
 */

SwapchainSupport getSwapchainSupportDetails(VkPhysicalDevice pPhysicalDevice,
                                            VkSurfaceKHR     pSurface) {
    SwapchainSupport swapchainDetails;

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

    /*upates formats vector to correct size*/
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
    /*upates present modes vector to correct size*/
    swapchainDetails.presentModes.resize(presentModeCount);

    vkGetPhysicalDeviceSurfacePresentModesKHR(pPhysicalDevice,
                                              pSurface,
                                              &presentModeCount,
                                              swapchainDetails.presentModes.data());
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
