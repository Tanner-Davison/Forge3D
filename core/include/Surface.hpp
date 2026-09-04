#pragma once
// clang-format off
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
// clang-format on

class Surface {
  public:
    // Constructor
    Surface(VkInstance pInstance, GLFWwindow* pWindow);

    // copy constructors && move constructors deletion
    Surface(const Surface&)            = delete;
    Surface& operator=(const Surface&) = delete;
    Surface(Surface&&)                 = delete;
    Surface& operator=(Surface&&)      = delete;

    // destructor
    ~Surface();

    // Handle
    VkSurfaceKHR handle() const;

  private:
    VkInstance   instance; // only used to create && destroy the surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
};
