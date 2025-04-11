#include "Surface.h"

Surface::Surface(VkInstance instance, GLFWwindow* window) :
    instance(instance)
{
    VK_CHECK(glfwCreateWindowSurface(instance, window, nullptr, &handle));
}

Surface::~Surface()
{
    vkDestroySurfaceKHR(instance, handle, nullptr);
}

Surface::Surface(Surface&& rhs) :
    instance(rhs.instance),
    handle(rhs.handle)
{
    rhs.handle = VK_NULL_HANDLE;
}

Surface& Surface::operator=(Surface&& rhs)
{
    vkDestroySurfaceKHR(instance, handle, nullptr);

    instance = rhs.instance;
    handle = rhs.handle;

    rhs.handle = VK_NULL_HANDLE;

    return *this;
}

VkSurfaceKHR Surface::getHandle()
{
    return handle;
}
