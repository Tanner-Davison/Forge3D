#pragma once
#include <optional>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamilyIndex;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pPhysicalDevice);
