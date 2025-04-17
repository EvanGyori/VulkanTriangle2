#pragma once

#include "VulkanWithGLFW.h"

class DebugUtilsMessenger
{
public:
    DebugUtilsMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    ~DebugUtilsMessenger();

    DebugUtilsMessenger(const DebugUtilsMessenger&) = delete;
    DebugUtilsMessenger& operator=(const DebugUtilsMessenger&) = delete;

    DebugUtilsMessenger(DebugUtilsMessenger&& rhs);
    DebugUtilsMessenger& operator=(DebugUtilsMessenger&& rhs);

    VkDebugUtilsMessengerEXT getHandle();

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT handle;
};
