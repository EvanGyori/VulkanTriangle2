#include <stdexcept>
#include <iostream>

#include "RenderingManager.h"
#include "Vertex.h"

static Vertex vertices[] = {
    {
	{ -0.5f, 0.5f, 1.0f },
	{ 1.0f, 0.0f, 0.0f }
    },
    {
	{ 0.5f, 0.5f, 1.0f },
	{ 0.0f, 1.0f, 0.0f }
    },
    {
	{ 0.0f, -0.5f, 1.0f },
	{ 0.0f, 0.0f, 1.0f }
    }
};

int main()
{
    try {
	RenderingManager manager;
    } catch (std::runtime_error error) {
	std::cout << error.what() << std::endl;
    }

    return 0;
}
