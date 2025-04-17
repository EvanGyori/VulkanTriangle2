#include <stdexcept>
#include <iostream>
#include <vector>

#include "RenderingManager.h"
#include "Vertex.h"

static std::vector<Vertex> vertices = {
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
	while (manager.shouldLoop()) {
	    manager.draw(vertices);
	}
    } catch (std::runtime_error error) {
	std::cout << error.what() << std::endl;
    }

    return 0;
}
