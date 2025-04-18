#pragma once

#include "Image.h"
#include "DeviceMemory.h"

// A linear image automatically associated to some device memory and host mapped
class LinearHostImage : public Image
{
public:
    LinearHostImage(VkDevice device, VkPhysicalDevice physicalDevice, GLFWwindow* window);

    LinearHostImage(LinearHostImage&& rhs);
    LinearHostImage& operator=(LinearHostImage&& rhs);

    void* getData();

    VkDeviceMemory getMemoryHandle();

private:
    DeviceMemory memory;
    void* data;
};
