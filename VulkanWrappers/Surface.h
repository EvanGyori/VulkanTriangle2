#pragma once

#include "VulkanWithGLFW.h"

class Surface
{
public:
    Surface(VkInstance instance, GLFWwindow* window);

    ~Surface();

    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    Surface(Surface&& rhs);
    Surface& operator=(Surface&& rhs);

    VkSurfaceKHR getHandle();

private:
    VkInstance instance;
    VkSurfaceKHR handle;
};
