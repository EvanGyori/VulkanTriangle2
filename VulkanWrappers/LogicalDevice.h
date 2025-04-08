#pragma once

#include <vector>
#include <cstddef>

#include "VulkanWithGLFW.h"

struct QueueFamily
{
    uint32_t queueFamilyIndex;
    std::vector<VkQueue> queues;
};

class LogicalDevice
{
public:
    LogicalDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo& createInfo);

    ~LogicalDevice();

    LogicalDevice(const LogicalDevice&) = delete;
    LogicalDevice& operator=(const LogicalDevice&) = delete;

    LogicalDevice(LogicalDevice&& rhs);
    LogicalDevice& operator=(LogicalDevice&& rhs);

    VkDevice getHandle();

    VkPhysicalDevice getPhysicalDevice();

    // index is the same index used in the createInfo.pQueueCreateInfos array
    const QueueFamily& getQueueFamily(size_t index);

    size_t getQueueFamilyCount();

protected:
    // Only use to delay instantiation is a subclass's constructor
    LogicalDevice();

private:
    VkDevice handle;
    VkPhysicalDevice physicalDevice;
    std::vector<QueueFamily> queueFamilies;
};
