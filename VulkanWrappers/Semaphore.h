#pragma once

#include "VulkanWithGLFW.h"

class Semaphore
{
public:
    Semaphore(VkDevice device, const VkSemaphoreCreateInfo& createInfo);

    ~Semaphore();

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    Semaphore(Semaphore&& rhs);
    Semaphore& operator=(Semaphore&& rhs);

    VkSemaphore getHandle();

private:
    VkDevice device;
    VkSemaphore handle;
};
