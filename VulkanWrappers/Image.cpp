#include "Image.h"

Image::Image(VkDevice device, const VkImageCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateImage(device, &createInfo, nullptr, &handle));
}

Image::~Image()
{
    vkDestroyImage(device, handle, nullptr);
}

Image::Image(Image&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Image& Image::operator=(Image&& rhs)
{
    vkDestroyImage(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkImage Image::getHandle()
{
    return handle;
}
