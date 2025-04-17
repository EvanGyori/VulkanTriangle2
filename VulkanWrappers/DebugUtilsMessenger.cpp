#include "DebugUtilsMessenger.h"

DebugUtilsMessenger::DebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& createInfo) :
    instance(instance)
{
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &handle));
}

DebugUtilsMessenger::~DebugUtilsMessenger()
{
    vkDestroyDebugUtilsMessengerEXT(instance, handle, nullptr);
}

DebugUtilsMessenger::DebugUtilsMessenger(DebugUtilsMessenger&& rhs) :
    instance(rhs.instance),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

DebugUtilsMessenger& DebugUtilsMessenger::operator=(DebugUtilsMessenger&& rhs)
{
    vkDestroyDebugUtilsMessengerEXT(instance, handle, nullptr);

    instance = rhs.instance;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkDebugUtilsMessengerEXT DebugUtilsMessenger::getHandle()
{
    return handle;
}
