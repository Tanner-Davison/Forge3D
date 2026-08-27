#pragma once
#include <optional>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamilyIndex;
    std::optional<uint32_t> presentFamilyIndex;

    bool isComplete() const {
        return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pPhysicalDevice, VkSurfaceKHR pSurface);
