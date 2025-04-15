#pragma once

#include "VulkanWithGLFW.h"

class Pipeline
{
public:
    // constructor for a graphics pipeline
    Pipeline(VkDevice device, const VkGraphicsPipelineCreateInfo& createInfo);

    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    Pipeline(Pipeline&& rhs);
    Pipeline& operator=(Pipeline&& rhs);

    VkPipeline getHandle();

private:
    VkDevice device;
    VkPipeline handle;
};
