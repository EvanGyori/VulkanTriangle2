#include "RenderPass.h"

RenderPass::RenderPass(VkDevice device, const VkRenderPassCreateInfo& createInfo)
{
    VK_CHECK(vkCreateRenderPass(device, &createInfo, nullptr, &handle));
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(device, handle, nullptr);
}

RenderPass::RenderPass(RenderPass&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

RenderPass& RenderPass::operator=(RenderPass&& rhs)
{
    vkDestroyRenderPass(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkRenderPass RenderPass::getHandle()
{
    return handle;
}
