#include "GLFWmanager.h"

#include <stdexcept>
#include <iostream>

GLFWmanager* GLFWmanager::instance = nullptr;

void glfwErrorCallback(int errorCode, const char* description)
{
    std::cerr << "Error Code: " << errorCode << ", " << description << std::endl;
}

void GLFWmanager::initVulkanAndGLFW()
{
    // Only initialize if haven't initialized before
    // A singleton object is used so that the destructor shuts down stuff
    // (although this isn't necessary on program termination)
    if (instance == nullptr) {
	instance = new GLFWmanager();
    }
}

GLFWmanager::~GLFWmanager()
{
    glfwTerminate();
}

GLFWmanager::GLFWmanager()
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (glfwInit() == GLFW_FALSE) {
	throw std::runtime_error("GLFW failed init");
    }

    if (glfwVulkanSupported == GLFW_FALSE) {
	throw std::runtime_error("Vulkan Loader not found");
    }
}
