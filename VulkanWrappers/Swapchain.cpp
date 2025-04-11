#include "Swapchain.h"

Swapchain::Swapchain(VkDevice device, const VkSwapchainCreateInfoKHR& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &handle));
}

Swapchain::~Swapchain()
{
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
