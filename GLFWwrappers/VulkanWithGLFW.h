/*
 * Uses GLFW to dynamically load the Vulkan Loader
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <sstream>
#include <string>

#define VK_CHECK(result) checkVkResult(result, __FILE__, __LINE__)

inline void checkVkResult(VkResult result, const char* filename, int lineNumber)
{
    if (result < 0) {
	std::stringstream ss;
	ss << "VULKAN ERROR: " << result << " in " << filename << " at line " << lineNumber;
	throw std::runtime_error(ss.str());
    }
}
