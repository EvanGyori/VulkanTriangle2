cmake_minimum_required(VERSION 3.20)
project(VulkanTriangle)

add_subdirectory(Utility)
add_subdirectory(GLFWwrappers)
add_subdirectory(VulkanWrappers)
add_subdirectory(Rendering)

add_executable(vulkanTriangle main.cpp)
target_link_libraries(vulkanTriangle PRIVATE Rendering)
