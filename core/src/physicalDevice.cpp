#include "physicalDevice.hpp"
#include <cstdint>
#include <print>
#include <vector>

inline std::string getDeviceTypeString(VkPhysicalDeviceType type) {
    switch (type) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "Other";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "Integrated GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "Discrete GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "Virtual GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "CPU";
        default:
            return "Unknown Device Type";
    }
}

VkPhysicalDevice getPhysicalDevice(VkInstance instance) {
    uint32_t device_count = 0;
    VkResult result       = vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    // 1st check if driver function itself failed to load
    if (result != VK_SUCCESS) {
        std::println(
            stderr, "Error: Failed to enumerate physical devices. VkResult: {}", static_cast<int>(result));
    }
    if (device_count == 0) {
        std::println(stderr, "Error: No Physical devices with vulkan support were found");
        return VK_NULL_HANDLE;
    }
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    if (!devices.empty()) {
        VkPhysicalDeviceProperties2 properties2{};

        properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        properties2.pNext = nullptr;

        vkGetPhysicalDeviceProperties2(devices[0], &properties2);

        // Unpack major, minor, and patch numbers from the packed API version integer
        uint32_t major = VK_API_VERSION_MAJOR(properties2.properties.apiVersion);
        uint32_t minor = VK_API_VERSION_MINOR(properties2.properties.apiVersion);
        uint32_t patch = VK_API_VERSION_PATCH(properties2.properties.apiVersion);

        std::println("\nDevice Name:\t{}", properties2.properties.deviceName);
        std::println(" API VERSION:\t{}.{}.{}", major, minor, patch);
        std::println(" Driver Version: {}", properties2.properties.driverVersion);
        std::println(" Vendor ID:\t{:#x}", properties2.properties.vendorID);
        std::println(" Device ID:\t{:#x}", properties2.properties.deviceID);
        std::println(" Device Type:\t{}", getDeviceTypeString(properties2.properties.deviceType));
    } else {
        return VK_NULL_HANDLE;
    }

    /*Display all properties of physical Device*/

    return devices[0];
}

// Debbugging only prints All devices does not return anything (SAFE TO DELETE)
void printPhysicalDevices(VkInstance instance) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());
    std::println("\n----All available Devices ----");
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties2 properties2{};
        properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        properties2.pNext = nullptr;
        vkGetPhysicalDeviceProperties2(device, &properties2);
        std::println("{}", properties2.properties.deviceName);
    }
}
