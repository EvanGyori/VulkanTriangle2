#include "Window.h"

#include <stdexcept>

Window::Window()
{
    GLFWmanager::initVulkanAndGLFW();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(800, 500, "I DID IT!!", nullptr, nullptr);
    if (window == nullptr) {
	throw std::runtime_error("Failed to create GLFW window");
    }
}

Window::~Window()
{
    glfwDestroyWindow(window);
}

GLFWwindow* Window::getHandle()
{
    return window;
}
