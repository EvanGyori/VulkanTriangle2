cmake_minimum_required(VERSION 3.20)

add_library(Rendering InstanceHelpers.cpp RenderingDevice.cpp ImageViewHelpers.cpp RenderPassHelpers.cpp SynchronizationHelpers.cpp RenderingFramebuffer.cpp RenderingSwapchain.cpp)
target_link_libraries(Rendering PUBLIC VulkanWrappers GLFWwrappers Utility)
target_include_directories(Rendering INTERFACE .)
