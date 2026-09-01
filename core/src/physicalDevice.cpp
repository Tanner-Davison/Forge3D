#include "physicalDevice.hpp"
#include "vk_enum_string_helper.h"
#include <cstdint>
#include <print>
#include <vector>

/* NOTE: We never communicate to the GPU directly with the
 * physical device we do that through the (logical device)
 * the Physical device is just the description of hardware*/

VkPhysicalDevice getPhysicalDevice(VkInstance instance) {
    uint32_t deviceCount = 0;
    VkResult result      = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // 1st check if driver function itself failed to load
    if (result != VK_SUCCESS) {
        std::println(stderr,
                     "Error: Failed to enumerate physical devices. VkResult: {}",
                     static_cast<int>(result));
        return VK_NULL_HANDLE;
    }
    if (deviceCount == 0) {
        std::println(stderr, "Error: No Physical devices with vulkan support were found");
        return VK_NULL_HANDLE;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDeviceProperties2 properties2{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = nullptr};

    vkGetPhysicalDeviceProperties2(devices[0], &properties2);

    // Unpack major, minor, and patch numbers from the packed API version integer
    uint32_t major = VK_API_VERSION_MAJOR(properties2.properties.apiVersion);
    uint32_t minor = VK_API_VERSION_MINOR(properties2.properties.apiVersion);
    uint32_t patch = VK_API_VERSION_PATCH(properties2.properties.apiVersion);

    std::println(" Device Name:\t{}", properties2.properties.deviceName);
    std::println(" API VERSION:\t{}.{}.{}", major, minor, patch);
    std::println(" Driver Version: {}", properties2.properties.driverVersion);
    std::println(" Vendor ID:\t{:#x}", properties2.properties.vendorID);
    std::println(" Device ID:\t{:#x}", properties2.properties.deviceID);
    // Uses vk_enum_string_helper.h
    std::println(" Device Type:\t{}\n",
                 string_VkPhysicalDeviceType(properties2.properties.deviceType));

    return devices[0];
}

// Debbugging only prints All devices does not return anything (SAFE TO DELETE)
void printPhysicalDevices(VkInstance instance) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());
    std::println("\n------| Devices Found |------");
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties2 properties2{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
            .pNext = nullptr};
        vkGetPhysicalDeviceProperties2(device, &properties2);
        std::println("{}", properties2.properties.deviceName);
    }
    std::println("------------------------------\n");
}
