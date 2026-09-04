#include "Surface.hpp"
#include <stdexcept>

Surface::Surface(VkInstance pInstance, GLFWwindow* pWindow) : instance(pInstance) {
    VkResult res = glfwCreateWindowSurface(pInstance, pWindow, nullptr, &surface);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Error: failed to create Window surface");
    };
};

Surface::~Surface() {
    if (this->surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    }
};

VkSurfaceKHR Surface::handle() const {
    return this->surface;
};
