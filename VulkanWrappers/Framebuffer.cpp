#include "Framebuffer.h"

Framebuffer::Framebuffer(VkDevice device, const VkFramebufferCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateFramebuffer(device, &createInfo, nullptr, &handle));
}

Framebuffer::~Framebuffer()
{
    if (device != VK_NULL_HANDLE)
	vkDestroyFramebuffer(device, handle, nullptr);
}

Framebuffer::Framebuffer(Framebuffer&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& rhs)
{
    if (device != VK_NULL_HANDLE)
	vkDestroyFramebuffer(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkFramebuffer Framebuffer::getHandle()
{
    return handle;
}

Framebuffer::Framebuffer() :
    device(VK_NULL_HANDLE),
    handle(VK_NULL_HANDLE)
{
}
