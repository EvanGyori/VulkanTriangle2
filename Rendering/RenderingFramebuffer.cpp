#include "RenderingFramebuffer.h"

#include "ImageViewHelpers.h"

RenderingFramebuffer::RenderingFramebuffer(VkDevice device, VkRenderPass renderPass, GLFWwindow* window, VkImage outputImage) :
    outputImageView(std::move(createRenderingImageView(device, outputImage)))
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkImageView imageView = outputImageView.getHandle();

    VkFramebufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &imageView;
    createInfo.width = static_cast<uint32_t>(width);
    createInfo.height = static_cast<uint32_t>(height);
    createInfo.layers = 1;

    // Default constructor of base class gets called which does not create a VkFramebuffer
    // So we must assign it with a newly created VkFramebuffer
    Framebuffer::operator=(std::move(Framebuffer(device, createInfo)));
}

RenderingFramebuffer::RenderingFramebuffer(RenderingFramebuffer&& rhs) :
    Framebuffer(std::move(rhs)),
    outputImageView(std::move(rhs.outputImageView))
{
}

RenderingFramebuffer& RenderingFramebuffer::operator=(RenderingFramebuffer&& rhs)
{
    Framebuffer::operator=(std::move(rhs));

    outputImageView = std::move(rhs.outputImageView);

    return *this;
}
