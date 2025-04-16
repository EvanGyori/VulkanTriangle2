#pragma once

#include <vector>

#include "Swapchain.h"
#include "Surface.h"
#include "RenderingFramebuffer.h"
#include "RenderingDevice.h"

class RenderingSwapchain : Swapchain
{
public:
    RenderingSwapchain(
	    VkInstance instance,
	    VkSurfaceKHR surface,
	    RenderingDevice& device,
	    VkRenderPass renderPass,
	    GLFWwindow* window);

    RenderingSwapchain(RenderingSwapchain&& rhs);
    RenderingSwapchain& operator=(RenderingSwapchain&& rhs);

    // imageIndex is the index received by acquiring the next image of this swapchain
    const RenderingFramebuffer& getFramebuffer(uint32_t imageIndex);

private:
    std::vector<RenderingFramebuffer> framebuffers;

    void setupFramebuffers(VkDevice device, VkRenderPass renderPass, GLFWwindow* window);
};
