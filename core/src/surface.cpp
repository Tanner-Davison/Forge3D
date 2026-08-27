#include "surface.hpp"
#include <print>

VkSurfaceKHR getWindowSurface(VkInstance pInstance, GLFWwindow* pWindow) {
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult     res     = glfwCreateWindowSurface(pInstance, pWindow, nullptr, &surface);
    if (res != VK_SUCCESS) {
        std::println(stderr, "Error: failed to create Window surface");
        return VK_NULL_HANDLE;
    }

    return surface;
};
