#include "ImageView.h"


ImageView::ImageView(VkDevice device, const VkImageViewCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateImageView(device, &createInfo, nullptr, &handle));
}

ImageView::~ImageView()
{
    vkDestroyImageView(device, handle, nullptr);
}

ImageView::ImageView(ImageView&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

ImageView& ImageView::operator=(ImageView&& rhs)
{
    vkDestroyImageView(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkImageView ImageView::getHandle()
{
    return handle;
}
