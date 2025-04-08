#pragma once

#include "GLFWmanager.h"

class Window
{
public:
    // Creates a window
    Window();

    // Destroys the window
    ~Window();

    GLFWwindow* getHandle();

private:
    GLFWwindow* window;
};
