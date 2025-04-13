#pragma once

#include <vector>
#include <functional>

#include "VulkanWithGLFW.h"

template<typename T>
std::vector<T> enumerateArray(std::function<VkResult(uint32_t*, T*)> enumerator)
{
    uint32_t count;
    VK_CHECK(enumerator(&count, nullptr));
    std::vector<T> elements(count);
    VK_CHECK(enumerator(&count, &elements.front()));

    return elements;
}

inline std::vector<VkLayerProperties> enumerateInstanceLayerProperties()
{
    return
	enumerateArray<VkLayerProperties>([](uint32_t* count, VkLayerProperties* elements) {
	    return vkEnumerateInstanceLayerProperties(count, elements);
	});
}

inline std::vector<VkExtensionProperties> enumerateInstanceExtensionProperties(const char* pLayerName)
{
    return
	enumerateArray<VkExtensionProperties>([pLayerName](uint32_t* count, VkExtensionProperties* elements) {
	    return vkEnumerateInstanceExtensionProperties(pLayerName, count, elements);
	});
}

inline std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance)
{
    return
	enumerateArray<VkPhysicalDevice>([instance](uint32_t* count, VkPhysicalDevice* elements) {
	    return vkEnumeratePhysicalDevices(instance, count, elements);
	});
}

inline std::vector<VkQueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice)
{
    return
	enumerateArray<VkQueueFamilyProperties>([physicalDevice](uint32_t* count, VkQueueFamilyProperties* elements) {
	    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, count, elements);
	    return VK_SUCCESS;
	});
}

inline std::vector<VkExtensionProperties> enumerateDeviceExtensionProperties(
	VkPhysicalDevice physicalDevice,
	const char* pLayerName)
{
    return
	enumerateArray<VkExtensionProperties>([physicalDevice, pLayerName](uint32_t* count, VkExtensionProperties* elements) {
	    return vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, count, elements);
	});
}

inline std::vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormatsKHR(
	VkPhysicalDevice physicalDevice,
	VkSurfaceKHR surface)
{
    return
	enumerateArray<VkSurfaceFormatKHR>([physicalDevice, surface](uint32_t* count, VkSurfaceFormatKHR* elements) {
	    return vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, count, elements);
	});
}

inline std::vector<VkImage> getSwapchainImagesKHR(
	VkDevice device,
	VkSwapchainKHR swapchain)
{
    return
	enumerateArray<VkImage>([device, swapchain](uint32_t* count, VkImage* elements) {
	    return vkGetSwapchainImagesKHR(device, swapchain, count, elements);
	});
}
