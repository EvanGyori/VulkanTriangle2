#include "Fence.h"

Fence::Fence(VkDevice device, const VkFenceCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateFence(device, &createInfo, nullptr, &handle));
}

Fence::~Fence()
{
    vkDestroyFence(device, handle, nullptr);
}

Fence::Fence(Fence&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Fence& Fence::operator=(Fence&& rhs)
{
    vkDestroyFence(device, handle, nullptr);

    device = rhs.device;

    handle = rhs.handle;
    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkFence Fence::getHandle()
{
    return handle;
}
