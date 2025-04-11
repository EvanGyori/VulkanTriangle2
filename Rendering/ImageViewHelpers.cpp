#include "ImageViewHelpers.h"

ImageView createRenderingImageView(VkDevice device, VkImage image)
{
    VkImageSubresourceRange subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.baseMipLevel = 0;
    subresource.levelCount = 1;
    subresource.baseArrayLayer = 0;
    subresource.layerCount = 1;

    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    createInfo.components = {}; // creates identity swizzle for all components
    createInfo.subresourceRange = subresource;

    return ImageView(device, &createInfo);
}
