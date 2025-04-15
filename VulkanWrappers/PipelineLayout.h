#pragma once

#include "VulkanWithGLFW.h"

class PipelineLayout
{
public:
    PipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo& createInfo);

    ~PipelineLayout();

    PipelineLayout(const PipelineLayout&) = delete;
    PipelineLayout& operator=(const PipelineLayout&) = delete;

    PipelineLayout(PipelineLayout&& rhs);
    PipelineLayout& operator=(PipelineLayout&& rhs);

    VkPipelineLayout getHandle();

private:
    VkDevice device;
    VkPipelineLayout handle;
};
