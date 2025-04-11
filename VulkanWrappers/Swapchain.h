#pragma once

#include "VulkanWithGLFW.h"

class Swapchain
{
public:
    Swapchain(VkDevice device, const VkSwapchainCreateInfoKHR& createInfo);

    ~Swapchain();

    Swapchain(const Swapchain&) = delete;
    Swapchain& operator=(const Swapchain&) = delete;

    Swapchain(Swapchain&& rhs);
    Swapchain& operator=(Swapchain&& rhs);

    VkSwapchainKHR getHandle();

private:
    VkDevice device;
    VkSwapchainKHR handle;
};
