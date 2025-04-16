#include "CommandPool.h"


CommandPool::CommandPool(VkDevice device, const VkCommandPoolCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateCommandPool(device, &createInfo, nullptr, &handle));
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(device, handle, nullptr);
}

CommandPool::CommandPool(CommandPool&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

CommandPool& CommandPool::operator=(CommandPool&& rhs)
{
    vkDestroyCommandPool(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkCommandPool CommandPool::getHandle()
{
    return handle;
}
