cmake_minimum_required(VERSION 3.20)

add_library(VulkanWrappers Instance.cpp)
target_link_libraries(VulkanWrappers PUBLIC GLFWwrappers)
target_include_directories(VulkanWrappers INTERFACE .)
