#include "swapchainSupport.hpp"
#include "queueFamilies.hpp"
#include <algorithm>
#include <cstdint>
#include <limits>
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

/*_____________ SWAPCHAIN SETTINGS____________________________*/
/* THREE Types of settings to determine when setting up a swapchain
 * -----------------------------------------------------------
 * 1. Surface format    (Color Depth)
 * 2. Presentation mode (conditions for "Swapping" images to the screen)
 * 3. Swap extent       (resolution of images in swapchain)
 */
VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    constexpr VkFormat        PREFFERED_FORMAT      = VK_FORMAT_B8G8R8A8_SRGB;
    constexpr VkColorSpaceKHR PREFFERED_COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == PREFFERED_FORMAT &&
            availableFormat.colorSpace == PREFFERED_COLOR_SPACE) {
            return availableFormat;
        }
    };

    return availableFormats[0];
};

VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
    constexpr VkPresentModeKHR PREFFERED_PRESENT_MODE = VK_PRESENT_MODE_MAILBOX_KHR;

    for (const auto& presentMode : availablePresentModes) {
        if (presentMode == PREFFERED_PRESENT_MODE) {
            return presentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities) {
    /*std::numeric_limits<uint32_t>::max() This is just 4294967295 (i.e. UINT32_MAX) the
     * largest value a uint32_t can hold.*/
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                   static_cast<uint32_t>(height)};

        actualExtent.width  = std::clamp(actualExtent.width,
                                        capabilities.minImageExtent.width,
                                        capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,
                                         capabilities.minImageExtent.height,
                                         capabilities.maxImageExtent.height);

        return actualExtent;
    }
};

VkSwapchainKHR createSwapchain(GLFWwindow*        pWindow,
                               VkDevice           pLogicalDevice,
                               SwapchainSupport&  swapchainSupport,
                               VkSurfaceKHR       surface,
                               QueueFamilyIndices indices) {
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR   presentMode = chooseSwapPresentMode(swapchainSupport.presentationModes);
    VkExtent2D         extent      = chooseSwapExtent(pWindow, swapchainSupport.capabilities);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    /*This makes sure we do not exceed the capable max image count*/
    if (swapchainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = surface;
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {indices.graphicsFamilyIndex.value(),
                                     indices.presentFamilyIndex.value()};

    if (indices.graphicsFamilyIndex != indices.presentFamilyIndex) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        /* VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families
         * without explicit ownership transfers.*/
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices   = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        /* VK_SHARING_MODE_EXCLUSIVE image is owned by one queue family at a time and ownership
         * must be explicitly transferred before using it in another queue family. This option
         * offers the best performance. */
        createInfo.queueFamilyIndexCount = 0; // optional
        createInfo.pQueueFamilyIndices   = nullptr;
    }
    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    /* We can specify that a certain transform should be applied to images in the swap chain if
     * it is supported (supportedTransforms in capabilities), like a 90 degree clockwise
     * rotation or horizontal flip. To specify that you do not want any transformation, simply
     * specify the current transformation.*/
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    /* The compositeAlpha field specifies if the alpha channel should be used for blending with
     * other windows in the window system. You'll almost always want to simply ignore the alpha
     * channel, hence VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR.*/
    createInfo.presentMode  = presentMode;
    createInfo.clipped      = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkSwapchainKHR swapChain;
    if (vkCreateSwapchainKHR(pLogicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        return nullptr;
    }
    return swapChain;
}
