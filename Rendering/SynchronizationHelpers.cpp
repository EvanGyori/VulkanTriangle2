#include "SynchronizationHelpers.h"

Semaphore createBinarySemaphore(VkDevice device)
{
    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    return Semaphore(device, createInfo);
}

Fence createFence(VkDevice device, bool initiallySignaled)
{
    VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (initiallySignaled) {
	createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    return Fence(device, createInfo);
}
