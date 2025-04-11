#pragma once

#include "VulkanWithGLFW.h"

class Framebuffer
{
public:
    Framebuffer(VkDevice device, const VkFramebufferCreateInfo& createInfo);

    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&& rhs);
    Framebuffer& operator=(Framebuffer&& rhs);

    VkFramebuffer getHandle();

private:
    VkDevice device;
    VkFramebuffer handle;
};
