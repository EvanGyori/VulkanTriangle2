#include "DebugUtilsMessenger.h"

PFN_vkCreateDebugUtilsMessengerEXT getFunction_vkCreateDebugUtilsMessengerEXT(VkInstance instance)
{
    return reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
	    vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
}

PFN_vkDestroyDebugUtilsMessengerEXT getFunction_vkDestroyDebugUtilsMessengerEXT(VkInstance instance)
{
    return reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
	    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
}

DebugUtilsMessenger::DebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& createInfo) :
    instance(instance)
{
    VK_CHECK(getFunction_vkCreateDebugUtilsMessengerEXT(instance)(instance, &createInfo, nullptr, &handle));
}

DebugUtilsMessenger::~DebugUtilsMessenger()
{
    getFunction_vkDestroyDebugUtilsMessengerEXT(instance)(instance, handle, nullptr);
}

DebugUtilsMessenger::DebugUtilsMessenger(DebugUtilsMessenger&& rhs) :
    instance(rhs.instance),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

DebugUtilsMessenger& DebugUtilsMessenger::operator=(DebugUtilsMessenger&& rhs)
{
    getFunction_vkDestroyDebugUtilsMessengerEXT(instance)(instance, handle, nullptr);

    instance = rhs.instance;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkDebugUtilsMessengerEXT DebugUtilsMessenger::getHandle()
{
    return handle;
}
