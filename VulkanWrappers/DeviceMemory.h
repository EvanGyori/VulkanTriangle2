#pragma once

#include "VulkanWithGLFW.h"

class DeviceMemory
{
public:
    DeviceMemory(VkDevice device, const VkMemoryAllocateInfo& allocInfo);

    ~DeviceMemory();

    DeviceMemory(const DeviceMemory&) = delete;
    DeviceMemory& operator=(const DeviceMemory&) = delete;

    DeviceMemory(DeviceMemory&& rhs);
    DeviceMemory& operator=(DeviceMemory&& rhs);

    VkDeviceMemory getHandle();

private:
    VkDevice device;
    VkDeviceMemory handle;
};
