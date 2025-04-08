#include "Instance.h"

#include "GLFWmanager.h"

Instance::Instance(const VkInstanceCreateInfo& createInfo)
{
    GLFWmanager::initVulkanAndGLFW();

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &handle));
}

Instance::~Instance()
{
    vkDestroyInstance(handle, nullptr);
}

Instance::Instance(Instance&& rhs) :
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Instance& Instance::operator=(Instance&& rhs)
{
    vkDestroyInstance(handle, nullptr);

    handle = rhs.handle;
    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkInstance Instance::getHandle()
{
    return handle;
}
