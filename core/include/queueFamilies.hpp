#pragma once
#include <optional>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    uint32_t                familyCount;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
