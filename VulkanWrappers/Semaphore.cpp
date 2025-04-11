#include "Semaphore.h"

Semaphore::Semaphore(VkDevice device, const VkSemaphoreCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateSemaphore(device, &createInfo, nullptr, &handle));
}

Semaphore::~Semaphore()
{
    vkDestroySemaphore(device, handle, nullptr);
}

Semaphore::Semaphore(Semaphore&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Semaphore& Semaphore::operator=(Semaphore&& rhs)
{
    vkDestroySemaphore(device, handle, nullptr);

    device = rhs.device;

    handle = rhs.handle;
    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkSemaphore Semaphore::getHandle()
{
    return handle;
}
