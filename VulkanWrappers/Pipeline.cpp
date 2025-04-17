#include "Pipeline.h"

#include <iostream>

Pipeline::Pipeline(
	VkDevice device,
	const VkGraphicsPipelineCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &handle));
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(device, handle, nullptr);
}

Pipeline::Pipeline(Pipeline&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Pipeline& Pipeline::operator=(Pipeline&& rhs)
{
    vkDestroyPipeline(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkPipeline Pipeline::getHandle()
{
    return handle;
}
