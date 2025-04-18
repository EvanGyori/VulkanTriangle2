#pragma once

#include "VulkanWithGLFW.h"

class Image
{
public:
    Image(VkDevice device, const VkImageCreateInfo& createInfo);

    ~Image();

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& rhs);
    Image& operator=(Image&& rhs);

    VkImage getHandle();

private:
    VkDevice device;
    VkImage handle;
};
