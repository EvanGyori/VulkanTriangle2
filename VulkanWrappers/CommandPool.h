#pragma once

#include "VulkanWithGLFW.h"

class CommandPool
{
public:
    CommandPool(VkDevice device, const VkCommandPoolCreateInfo& createInfo);

    ~CommandPool();

    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;

    CommandPool(CommandPool&& rhs);
    CommandPool& operator=(CommandPool&& rhs);

    VkCommandPool getHandle();

private:
    VkDevice device;
    VkCommandPool handle;
};
