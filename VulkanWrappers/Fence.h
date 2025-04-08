#pragma once

#include "VulkanWithGLFW.h"

class Fence
{
public:
    Fence(VkDevice device, const VkFenceCreateInfo& createInfo);

    ~Fence();

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

    Fence(Fence&& rhs);
    Fence& operator=(Fence&& rhs);

    VkFence getHandle();

private:
    VkDevice device;
    VkFence handle;
};
