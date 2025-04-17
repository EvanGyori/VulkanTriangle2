#include "ShaderModule.h"

ShaderModule::ShaderModule(VkDevice device, const VkShaderModuleCreateInfo& createInfo) :
    device(device)
{
    VK_CHECK(vkCreateShaderModule(device, &createInfo, nullptr, &handle));
}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(device, handle, nullptr);
}

ShaderModule::ShaderModule(ShaderModule&& rhs) :
    device(rhs.device),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

ShaderModule& ShaderModule::operator=(ShaderModule&& rhs)
{
    vkDestroyShaderModule(device, handle, nullptr);

    device = rhs.device;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkShaderModule ShaderModule::getHandle()
{
    return handle;
}
