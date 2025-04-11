#include "RenderPassHelpers.h"

#include <vector>

std::vector<VkAttachmentDescription> getAttachmentDescriptions();

std::vector<VkSubpassDescription> getSubpassDescriptions();

RenderPass createRenderingRenderPass(VkDevice device)
{
    auto attachmentDescriptions = getAttachmentDescriptions();
    auto subpassDescriptions = getSubpassDescriptions();

    VkRenderPassCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = attachmentDescriptions.size();
    createInfo.pAttachments = &attachmentDescriptions.front();
    createInfo.subpassCount = subpassDescriptions.size();
    createInfo.pSubpasses = &subpassDescriptions.front();

    return RenderPass(device, createInfo);
}

std::vector<VkAttachmentDescription> getAttachmentDescriptions()
{
    VkAttachmentDescription output = {};
    output.format = VK_FORMAT_R8G8B8A8_SRGB;
    output.samples = VK_SAMPLE_COUNT_1_BIT;
    output.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    output.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    output.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    output.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    return { output };
}

std::vector<VkSubpassDescription> getSubpassDescriptions()
{
    VkAttachmentReference outputReference;
    outputReference.attachment = 0;
    outputReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &outputReference;

    return { subpass };
}
