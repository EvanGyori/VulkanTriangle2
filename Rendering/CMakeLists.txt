cmake_minimum_required(VERSION 3.20)

add_library(Rendering InstanceHelpers.cpp)
target_link_libraries(Rendering PUBLIC VulkanWrappers GLFWwrappers Utility)
target_include_directories(Rendering INTERFACE .)
