cmake_minimum_required(VERSION 3.20)

find_package(glfw NAMES glfw3 GLFW REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(VulkanLoader REQUIRED)

add_library(GLFWwrappers GLFWmanager.cpp Window.cpp)
target_link_libraries(GLFWwrappers PUBLIC glfw Vulkan::Headers Vulkan::Loader)
target_include_directories(GLFWwrappers INTERFACE .)
