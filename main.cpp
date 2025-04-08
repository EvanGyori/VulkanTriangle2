#include "Window.h"
#include "InstanceHelpers.h"

#include <stdexcept>
#include <iostream>

int main()
{
    try {
	Window window;

	Instance instance = createRenderingInstance();

	while (!glfwWindowShouldClose(window.getHandle())) {
	    glfwPollEvents();
	}
    } catch (std::runtime_error error) {
	std::cout << error.what() << std::endl;
    }

    return 0;
}
