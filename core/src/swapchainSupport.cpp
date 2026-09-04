#include "swapchainSupport.hpp"
#include <cstdint>
#include <print>

/*
 *   Checks for Capabilites ->  Surface  formats -> Present Modes
 *   && returns all in swapchainDetails
 *
 */

SwapchainSupport getSwapchainSupportDetails(VkPhysicalDevice pPhysicalDevice,
                                            VkSurfaceKHR     pSurface) {
    SwapchainSupport swapchainDetails;

    /*GET swapchainDetails.capabilities ----------------------------*/
    VkResult capabilitiesRes =
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pPhysicalDevice,
                                                  pSurface,
                                                  &swapchainDetails.capabilities);
    if (capabilitiesRes != VK_SUCCESS) {
        std::println(stderr, "Error: Could not see Device Surface Capabilities.");
    }

    /* GET swapchainDetails.formats ------------------------------*/
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pPhysicalDevice, pSurface, &formatCount, nullptr);

    /*upates swapchainDetails.formats vector with the correct size*/
    swapchainDetails.formats.resize(formatCount);
    VkResult formatRes = vkGetPhysicalDeviceSurfaceFormatsKHR(pPhysicalDevice,
                                                              pSurface,
                                                              &formatCount,
                                                              swapchainDetails.formats.data());
    if (formatRes != VK_SUCCESS) {
        std::println(stderr, "Error: Failed to get physical device surface format KHR");
    }

    /*GET swapchainDetails.presenationModes ---------------------------*/
    uint32_t presentationModeCount = 0;
    VkResult presentationRes =
        vkGetPhysicalDeviceSurfacePresentModesKHR(pPhysicalDevice,
                                                  pSurface,
                                                  &presentationModeCount,
                                                  nullptr);
    /*upates present modes vector to correct size*/
    swapchainDetails.presentationModes.resize(presentationModeCount);

    vkGetPhysicalDeviceSurfacePresentModesKHR(pPhysicalDevice,
                                              pSurface,
                                              &presentationModeCount,
                                              swapchainDetails.presentationModes.data());
    if (presentationRes != VK_SUCCESS) {
        std::println(stderr, "Error: Could not find present modes count");
    }

    /*If any member of SwapchainSupport struct fails it prints an Error & still sends struct */
    if (!swapchainDetails.isComplete()) {
        std::println(stderr, "Error: swapchain details failed to find any of the following:");
        std::println(stderr, "formats");
        std::println(stderr, "presentationModes");
        return swapchainDetails;
    }
    return swapchainDetails;
};
