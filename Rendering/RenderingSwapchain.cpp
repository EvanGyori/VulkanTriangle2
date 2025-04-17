#include "RenderingSwapchain.h"

#include <utility>

#include "Utility.h"
#include "EnumerationHelpers.h"

uint32_t getMinImageCount(VkSurfaceCapabilitiesKHR surfaceCapabilities);

VkExtent2D getImageExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, GLFWwindow* window);

VkSurfaceTransformFlagBitsKHR getPreTransform(VkSurfaceCapabilitiesKHR surfaceCapabilities);

VkCompositeAlphaFlagBitsKHR getCompositeAlpha(VkSurfaceCapabilitiesKHR surfaceCapabilities);

RenderingSwapchain::RenderingSwapchain(
	VkInstance instance,
	VkSurfaceKHR surface,
	RenderingDevice& device,
	VkRenderPass renderPass,
	GLFWwindow* window)
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.getPhysicalDevice(), surface, &surfaceCapabilities));

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = getMinImageCount(surfaceCapabilities);

    // RenderingDevice only picks a physical device with these supported format and colorSpace
    createInfo.imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    createInfo.imageExtent = getImageExtent(surfaceCapabilities, window);
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    const QueueFamily& graphicsFamily = device.getGraphicsQueueFamily();
    const QueueFamily& presentFamily = device.getPresentQueueFamily();
    uint32_t queueFamilyIndices[2] = { graphicsFamily.queueFamilyIndex, presentFamily.queueFamilyIndex };
    if (graphicsFamily.queueFamilyIndex == presentFamily.queueFamilyIndex) {
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; 
    } else {
	createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	createInfo.queueFamilyIndexCount = 2;
	createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    createInfo.preTransform = getPreTransform(surfaceCapabilities);
    createInfo.compositeAlpha = getCompositeAlpha(surfaceCapabilities);
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    Swapchain::operator=(Swapchain(device.getHandle(), createInfo));

    setupFramebuffers(device.getHandle(), renderPass, window);
}

RenderingSwapchain::RenderingSwapchain(RenderingSwapchain&& rhs) :
    Swapchain(std::move(rhs)),
    framebuffers(std::move(rhs.framebuffers))
{
}

RenderingSwapchain& RenderingSwapchain::operator=(RenderingSwapchain&& rhs)
{
    framebuffers = std::move(rhs.framebuffers);

    Swapchain::operator=(std::move(rhs));

    return *this;
}

RenderingFramebuffer& RenderingSwapchain::getFramebuffer(uint32_t imageIndex)
{
    return framebuffers[imageIndex];
}

void RenderingSwapchain::setupFramebuffers(VkDevice device, VkRenderPass renderPass, GLFWwindow* window)
{
    std::vector<VkImage> images = getSwapchainImagesKHR(device, getHandle());
    for (int i = 0; i < images.size(); ++i) {
	framebuffers.emplace_back(device, renderPass, window, images[i]);
    }
}

uint32_t getMinImageCount(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
    uint32_t minImageCount = 2;
    if (surfaceCapabilities.minImageCount > minImageCount) {
	return surfaceCapabilities.minImageCount;
    }

    if (surfaceCapabilities.maxImageCount != 0 && surfaceCapabilities.maxImageCount < minImageCount) {
	return surfaceCapabilities.maxImageCount;
    }

    return minImageCount;
}

VkExtent2D getImageExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, GLFWwindow* window)
{
    if (surfaceCapabilities.currentExtent.width != 0xFFFFFFFF
	|| surfaceCapabilities.currentExtent.height != 0xFFFFFFFF) {
	return surfaceCapabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D extent = {
	static_cast<uint32_t>(width),
	static_cast<uint32_t>(height)
    };

    extent.width = clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
    extent.height = clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

    return extent;
}

VkSurfaceTransformFlagBitsKHR getPreTransform(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
    return surfaceCapabilities.currentTransform;
}

VkCompositeAlphaFlagBitsKHR getCompositeAlpha(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
    VkCompositeAlphaFlagsKHR supported = surfaceCapabilities.supportedCompositeAlpha;
    if (supported & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) {
	return VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    } else if (supported & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
	return VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    } else if (supported & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
	return VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
    } else {
	return VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
    }
}
