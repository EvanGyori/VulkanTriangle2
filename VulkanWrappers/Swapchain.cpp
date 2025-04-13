#include "Swapchain.h"

Swapchain::Swapchain(VkDevice device, const VkSwapchainCreateInfoKHR& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &handle));
}

Swapchain::~Swapchain()
{
    if (device != VK_NULL_HANDLE)
	vkDestroySwapchainKHR(device, handle, nullptr);
}

Swapchain::Swapchain(Swapchain&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Swapchain& Swapchain::operator=(Swapchain&& rhs)
{
    if (device != VK_NULL_HANDLE)
	vkDestroySwapchainKHR(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkSwapchainKHR Swapchain::getHandle()
{
    return handle;
}

Swapchain::Swapchain() :
    device(VK_NULL_HANDLE),
    handle(VK_NULL_HANDLE)
{
}
