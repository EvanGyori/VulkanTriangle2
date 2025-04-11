#pragma once

#include "VulkanWithGLFW.h"

class ImageView
{
public:
    ImageView(VkDevice device, const VkImageViewCreateInfo& createInfo);

    ~ImageView();

    ImageView(const ImageView&) = delete;
    ImageView& operator=(const ImageView&) = delete;

    ImageView(ImageView&& rhs);
    ImageView& operator=(ImageView&& rhs);

    VkImageView getHandle();

private:
    VkDevice device;
    VkImageView handle;

};
