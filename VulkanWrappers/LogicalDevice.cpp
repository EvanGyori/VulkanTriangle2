#include "LogicalDevice.h"

#include <utility>

// Retrieve all queue handles from a device after its creation. Requires the createInfo
std::vector<QueueFamily> getQueueHandles(VkDevice device, const VkDeviceCreateInfo& createInfo);

LogicalDevice::LogicalDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo& createInfo) :
    handle(VK_NULL_HANDLE),
    physicalDevice(physicalDevice),
    queueFamilies()
{
    VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &handle));
    queueFamilies = getQueueHandles(handle, createInfo);
}

LogicalDevice::~LogicalDevice()
{
    vkDestroyDevice(handle, nullptr);
}

LogicalDevice::LogicalDevice(LogicalDevice&& rhs) :
    handle(rhs.handle),
    physicalDevice(rhs.physicalDevice),
    queueFamilies(std::move(rhs.queueFamilies))
{
    rhs.handle = VK_NULL_HANDLE;
    rhs.physicalDevice = VK_NULL_HANDLE;
}

LogicalDevice& LogicalDevice::operator=(LogicalDevice&& rhs)
{
    vkDestroyDevice(handle, nullptr);

    handle = rhs.handle;
    rhs.handle = VK_NULL_HANDLE;

    physicalDevice = rhs.physicalDevice;
    rhs.physicalDevice = VK_NULL_HANDLE;

    queueFamilies = std::move(rhs.queueFamilies);

    return *this;
}

VkDevice LogicalDevice::getHandle()
{
    return handle;
}

VkPhysicalDevice LogicalDevice::getPhysicalDevice()
{
    return physicalDevice;
}

const QueueFamily& LogicalDevice::getQueueFamily(size_t index)
{
    return queueFamilies[index];
}

size_t LogicalDevice::getQueueFamilyCount()
{
    return queueFamilies.size();
}

LogicalDevice::LogicalDevice() :
    handle(VK_NULL_HANDLE),
    physicalDevice(VK_NULL_HANDLE),
    queueFamilies()
{
}

std::vector<QueueFamily> getQueueHandles(VkDevice device, const VkDeviceCreateInfo& createInfo)
{
    std::vector<QueueFamily> queueFamilies(createInfo.queueCreateInfoCount);

    for (int i = 0; i < createInfo.queueCreateInfoCount; i++) {
	const VkDeviceQueueCreateInfo queueInfo = createInfo.pQueueCreateInfos[i];
	queueFamilies[i] = {
	    queueInfo.queueFamilyIndex,
	    std::vector<VkQueue>(queueInfo.queueCount)
	};

	// Get all queue handles created under same queue family in same queueCreateInfo
	for (int j = 0; j < queueInfo.queueCount; j++) {
	    VkQueue queue;
	    vkGetDeviceQueue(device, queueInfo.queueFamilyIndex, j, &queue);
	    queueFamilies[i].queues[j] = queue;
	}
    }

    return queueFamilies;
}
