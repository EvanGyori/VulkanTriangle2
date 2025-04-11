#pragma once

#include "VulkanWithGLFW.h"

class RenderPass
{
public:
    RenderPass(VkDevice device, const VkRenderPassCreateInfo& createInfo);

    ~RenderPass();

    RenderPass(const RenderPass&) = delete;
    RenderPass& operator=(const RenderPass&) = delete;

    RenderPass(RenderPass&& rhs);
    RenderPass& operator=(RenderPass&& rhs);

    VkRenderPass getHandle();
    
private:
    VkDevice device;
    VkRenderPass handle;
};
