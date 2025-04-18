#include "DeviceMemory.h"

DeviceMemory::DeviceMemory(VkDevice device, const VkMemoryAllocateInfo& allocInfo) :
    device(device)
{
    VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &handle));
}

DeviceMemory::~DeviceMemory()
{
    vkFreeMemory(device, handle, nullptr);
}

DeviceMemory::DeviceMemory(DeviceMemory&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

DeviceMemory& DeviceMemory::operator=(DeviceMemory&& rhs)
{
    vkFreeMemory(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkDeviceMemory DeviceMemory::getHandle()
{
    return handle;
}
