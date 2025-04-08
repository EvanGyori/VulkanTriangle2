/*
 * Singleton class used to check if GLFW has been initialized yet
 * and does so if it hasn't. GLFW also dynamically loads in the Vulkan Loader
 */

#pragma once

#include "VulkanWithGLFW.h"

class GLFWmanager
{
public:
    static void initVulkanAndGLFW();
    
    // Destructor of the singleton instance handles shutting down stuff
    ~GLFWmanager();

private:
    static GLFWmanager* instance;

    // Default constructor of the singleton instance handles initialization
    GLFWmanager();
};
