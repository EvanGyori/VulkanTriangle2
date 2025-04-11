#include "RenderingDevice.h"

#include <vector>
#include <stdexcept>

#include "Utility.h"

const float defaultQueuePriority = 1.0f;

std::vector<const char*> getRequiredDeviceExtensions();

VkPhysicalDeviceFeatures getRequiredFeatures();

std::vector<VkDeviceQueueCreateInfo> getQueueCreateInfos(
	VkInstance instance,
	VkPhysicalDevice physicalDevice);

bool doesPhysicalDeviceSupportExtensions(
	VkPhysicalDevice physicalDevice,
	const std::vector<const char*> extensions);

bool doesPhysicalDeviceSupportFeatures(
	VkPhysicalDevice physicalDevice,
	VkPhysicalDeviceFeatures features);

bool doesPhysicalDeviceSupportRequiredQueueCapabilities(
	VkInstance instance,
	VkPhysicalDevice physicalDevice);

bool doesPhysicalDeviceSupportRequiredSurfaceFormats(
	VkPhysicalDevice physicalDevice,
	VkSurfaceKHR surface);

bool isPhysicalDeviceSuitable(
	VkInstance instance,
	VkPhysicalDevice physicalDevice,
	VkSurfaceKHR surface);

VkPhysicalDevice findPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

RenderingDevice::RenderingDevice(VkInstance instance, VkSurfaceKHR surface)
{
    VkPhysicalDevice physicalDevice = findPhysicalDevice(instance, surface);
    auto queueCreateInfos = getQueueCreateInfos(instance, physicalDevice);
    auto extensions = getRequiredDeviceExtensions();
    auto features = getRequiredFeatures();

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = &queueCreateInfos.front();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = &extensions.front();
    createInfo.pEnabledFeatures = &features;

    LogicalDevice::operator=(LogicalDevice(physicalDevice, createInfo));
}

RenderingDevice::RenderingDevice(RenderingDevice&& rhs) :
    LogicalDevice(rhs),
    graphicsFamilyIndex(rhs.graphicsFamilyIndex),
    presentFamilyIndex(rhs.presentFamilyIndex)
{
    rhs.graphicsFamilyIndex = 0;
    rhs.presentFamilyIndex = 0;
}

RenderingDevice& RenderingDevice::operator=(RenderingDevice&& rhs)
{
    LogicalDevice::operator=(rhs);

    graphicsFamilyIndex = rhs.graphicsFamilyIndex;
    rhs.graphicsFamilyIndex = 0;

    presentFamilyIndex = rhs.presentFamilyIndex;
    rhs.presentFamilyIndex = 0;

    return *this;
}

VkQueue RenderingDevice::getGraphicsQueue()
{
    return getQueueFamily(graphicsFamilyIndex).queues[0];
}

VkQueue RenderingDevice::getPresentQueue()
{
    return getQueueFamily(presentFamilyIndex).queues[0];
}

std::vector<const char*> getRequiredDeviceExtensions()
{
    return { "VK_KHR_swapchain" };
}

VkPhysicalDeviceFeatures getRequiredFeatures()
{
    return {};
}

std::vector<VkDeviceQueueCreateInfo> getQueueCreateInfos(VkInstance instance, VkPhysicalDevice physicalDevice)
{
    std::vector<VkQueueFamilyProperties> queueFamilies = getPhysicalDeviceQueueFamilyProperties(physicalDevice);
    uint32_t graphicsFamily;
    uint32_t presentFamily;

    for (int i = 0; i < queueFamilies.size(); ++i) {
	if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
	    graphicsFamily = i;
	}

	if (glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, i) == GLFW_TRUE) {
	    presentFamily = i;
	}

	if (graphicsFamily == presentFamily) {
	    break;
	}
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos((graphicsFamily == presentFamily) ? 1 : 2);
    
    queueCreateInfos[0] = {};
    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = graphicsFamily;
    queueCreateInfos[0].queueCount = 1;
    queueCreateInfos[0].pQueuePriorities = &defaultQueuePriority;

    if (graphicsFamily != presentFamily) {
	queueCreateInfos[1] = queueCreateInfos[0];
	queueCreateInfos[1].queueFamilyIndex = presentFamily;
    }

    return queueCreateInfos;
}

bool doesPhysicalDeviceSupportExtensions(
	VkPhysicalDevice physicalDevice,
	const std::vector<const char*> extensions)
{
    std::vector<VkExtensionProperties> supportedExtensions = enumerateDeviceExtensionProperties(physicalDevice, nullptr);
    
    std::vector<const char*> supportedExtensionNames(supportedExtensions.size());
    for (int i = 0; i < supportedExtensions.size(); i++) {
	supportedExtensionNames[i] = supportedExtensions[i].extensionName;
    }

    return isASubsetOfB(extensions, supportedExtensions);
}

bool doesPhysicalDeviceSupportFeatures(
	VkPhysicalDevice physicalDevice,
	VkPhysicalDeviceFeatures features)
{
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
    return features & supportedFeatures == features;
}

bool doesPhysicalDeviceSupportRequiredQueueCapabilities(VkInstance instance, VkPhysicalDevice physicalDevice)
{
    std::vector<VkQueueFamilyProperties> queueFamilies = getPhysicalDeviceQueueFamilyProperties(physicalDevice);

    bool hasGraphicsSupport = false;
    bool hasPresentationSupport = false;
    for (int i = 0; i < queueFamilies.size(); ++i) {
	if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
	    hasGraphicsSupport = true;
	}

	if (glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, i) == GLFW_TRUE) {
	    hasPresentationSupport = true;
	}
    }

    return hasGraphicsSupport && hasPresentingSupport;
}

bool doesPhysicalDeviceSupportRequiredSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    std::vector<VkSurfaceFormatKHR> supportedFormats = getPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface);

    for (auto format : supportedFormats) {
	if (format.format == VK_FORMAT_R8G8B8A8_SRGB
	    && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
	    return true;
	}
    }

    return false;
}

bool isPhysicalDeviceSuitable(VkInstance instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    return doesPhysicalDeviceSupportExtensions(physicalDevice, getRequiredDeviceExtensions())
	&& doesPhysicalDeviceSupportFeatures(physicalDevice, getRequiredFeatures())
	&& doesPhysicalDeviceSupportRequiredQueueCapabilities(instance, physicalDevice)
	&& doesPhysicalDeviceSupportRequiredSurfaceFormats(physicalDevice surface);
}

VkPhysicalDevice findPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    std::vector<VkPhysicalDevice> physicalDevices = enumeratePhysicalDevices(instance);

    for (auto physicalDevice : physicalDevices) {
	if (isPhysicalDeviceSuitable(instance, physicalDevice, surface)) {
	    return physicalDevice;
	}
    }

    throw std::runtime_error("No suitable physical devices found");
    return VK_NULL_HANDLE;
}
