#pragma once

#include "ImageView.h"
#include "Framebuffer.h"

class RenderingFramebuffer : public Framebuffer
{
public:
    RenderingFramebuffer(VkDevice device, VkRenderPass renderPass, GLFWwindow* window, VkImage outputImage);

    RenderingFramebuffer(RenderingFramebuffer&& rhs);
    RenderingFramebuffer& operator=(RenderingFramebuffer&& rhs);

private:
    ImageView outputImageView;
};
