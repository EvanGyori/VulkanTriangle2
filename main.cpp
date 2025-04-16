#include <stdexcept>
#include <iostream>

#include "RenderingManager.h"

int main()
{
    try {
	RenderingManager manager;
    } catch (std::runtime_error error) {
	std::cout << error.what() << std::endl;
    }

    return 0;
}
