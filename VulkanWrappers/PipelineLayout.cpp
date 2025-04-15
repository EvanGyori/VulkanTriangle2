#include "PipelineLayout.h"

PipelineLayout::PipelineLayout(
	VkDevice device,
	const VkPipelineLayoutCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreatePipelineLayout(device, &createInfo, nullptr, &handle));
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyPipelineLayout(device, handle, nullptr);
}

PipelineLayout::PipelineLayout(PipelineLayout&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

PipelineLayout& PipelineLayout::operator=(PipelineLayout&& rhs)
{
    vkDestroyPipelineLayout(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkPipelineLayout PipelineLayout::getHandle()
{
    return handle;
}
