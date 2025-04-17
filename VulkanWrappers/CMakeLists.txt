cmake_minimum_required(VERSION 3.20)

add_library(VulkanWrappers Instance.cpp LogicalDevice.cpp RenderPass.cpp Fence.cpp Semaphore.cpp ImageView.cpp Framebuffer.cpp Surface.cpp Swapchain.cpp PipelineLayout.cpp Pipeline.cpp CommandPool.cpp DebugUtilsMessenger.cpp)
target_link_libraries(VulkanWrappers PUBLIC GLFWwrappers)
target_include_directories(VulkanWrappers INTERFACE .)
