#include "Fence.h"

Fence::Fence(VkDevice device, const VkFenceCreateInfo& createInfo)
{
    VK_CHECK(vkCreatefence(device, &createInfo, nullptr, &handle));
}

Fence::~Fence()
{
    vkDestroyFence(device, handle, nullptr);
}

Fence::Fence(Fence&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.device = VK_NULL_HANDLE;
    rhs.handle = VK_NULL_HANDLE;
}

Fence& Fence::operator=(Fence&& rhs)
{
    vkDestroyFence(device, handle, nullptr);

    device = rhs.device;
    rhs.device = VK_NULL_HANDLE;

    handle = rhs.handle;
    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkFence Fence::getHandle()
{
    return handle;
}
