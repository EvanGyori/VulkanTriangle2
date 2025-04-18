#include <stdexcept>
#include <iostream>
#include <vector>

#include "RenderingManager.h"
#include "Vertex.h"

#include "steam/steam_api.h"

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
    if (!SteamAPI_Init()) {
	std::cerr << "STEAM failed to init\n";
	return 1;
    }

    try {
	RenderingManager manager;
	while (manager.shouldLoop()) {
	    SteamAPI_RunCallbacks();
	    manager.draw(vertices);
	}
    } catch (std::runtime_error error) {
	std::cout << error.what() << std::endl;
    }

    SteamAPI_Shutdown();

    return 0;
}
