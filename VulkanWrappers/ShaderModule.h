#pragma once

#include "VulkanWithGLFW.h"

class ShaderModule
{
public:
    ShaderModule(VkDevice device, const VkShaderModuleCreateInfo& createInfo);

    ~ShaderModule();

    ShaderModule(const ShaderModule&) = delete;
    ShaderModule& operator=(const ShaderModule&) = delete;

    ShaderModule(ShaderModule&& rhs);
    ShaderModule& operator=(ShaderModule&& rhs);

    VkShaderModule getHandle();

private:
    VkDevice device;
    VkShaderModule handle;
};
