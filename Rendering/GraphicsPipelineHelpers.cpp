#include "GraphicsPipelineHelpers.h"

#include <vector>
#include <cstddef>

#include "Utility.h"
#include "Vertex.h"

std::vector<VkShaderModuleCreateInfo> getShaderModuleCreateInfos(
	const std::vector<char>& vertexShaderData,
	const std::vector<char>& fragmentShaderData);

std::vector<VkPipelineShaderStageCreateInfo> getShaderStageInfos(
	const std::vector<VkShaderModuleCreateInfo>& moduleInfos);

std::vector<VkVertexInputBindingDescription> getVertexBindings();

std::vector<VkVertexInputAttributeDescription> getAttributeBindings();

VkPipelineVertexInputStateCreateInfo getVertexInputInfo(
	const std::vector<VkVertexInputBindingDescription>& bindings,
	const std::vector<VkVertexInputAttributeDescription>& attributes);

VkPipelineInputAssemblyStateCreateInfo getInputAssemblyInfo();

VkViewport getViewport(GLFWwindow* window);

// Returns a scissor that covers the entire window's framebuffer
VkRect2D getScissor(GLFWwindow* window);

VkPipelineViewportStateCreateInfo getViewportInfo(
	const VkViewport& viewport,
	const VkRect2D& scissor);

VkPipelineViewportStateCreateInfo getViewportInfo();

VkPipelineRasterizationStateCreateInfo getRasterizationInfo();

VkPipelineMultisampleStateCreateInfo getMultisampleInfo();

VkPipelineColorBlendAttachmentState getColorBlendAttachmentInfo();

VkPipelineColorBlendStateCreateInfo getColorBlendInfo(
	const VkPipelineColorBlendAttachmentState& colorBlendAttachmentInfo);

PipelineLayout createEmptyPipelineLayout(VkDevice device)
{
    VkPipelineLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    return PipelineLayout(device, createInfo);
}

Pipeline createRenderingPipeline(
	VkDevice device,
	VkRenderPass renderPass,
	VkPipelineLayout layout,
	GLFWwindow* window)
{
    auto vertexShaderData = getFileData("vert.spv");
    auto fragmentShaderData = getFileData("frag.spv");
    auto shaderModuleInfos = getShaderModuleCreateInfos(vertexShaderData, fragmentShaderData);
    auto shaderStageInfos = getShaderStageInfos(shaderModuleInfos);

    auto bindings = getVertexBindings();
    auto attributes = getAttributeBindings();
    auto vertexInputInfo = getVertexInputInfo(bindings, attributes);

    auto inputAssemblyInfo = getInputAssemblyInfo();

    auto viewport = getViewport(window);
    auto scissor = getScissor(window);
    auto viewportInfo = getViewportInfo(viewport, scissor);

    auto rasterizationInfo = getRasterizationInfo();
    auto multisampleInfo = getMultisampleInfo();

    auto colorBlendAttachmentInfo = getColorBlendAttachmentInfo();
    auto colorBlendInfo = getColorBlendInfo(colorBlendAttachmentInfo);

    VkGraphicsPipelineCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.stageCount = shaderStageInfos.size();
    createInfo.pStages = &shaderStageInfos.front();
    createInfo.pVertexInputState = &vertexInputInfo;
    createInfo.pInputAssemblyState = &inputAssemblyInfo;
    createInfo.pViewportState = &viewportInfo;
    createInfo.pRasterizationState = &rasterizationInfo;
    createInfo.pMultisampleState = &multisampleInfo;
    createInfo.pColorBlendState = &colorBlendInfo;
    createInfo.layout = layout;
    createInfo.renderPass = renderPass;
    createInfo.subpass = 0;

    return Pipeline(device, createInfo);
}

std::vector<VkShaderModuleCreateInfo> getShaderModuleCreateInfos(
	const std::vector<char>& vertexShaderData,
	const std::vector<char>& fragmentShaderData)
{
    std::vector<VkShaderModuleCreateInfo> createInfos = { {}, {} };
    
    createInfos[0].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfos[0].codeSize = vertexShaderData.size() / sizeof(uint32_t);
    createInfos[0].pCode = reinterpret_cast<const uint32_t*>(&vertexShaderData.front());

    createInfos[1].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfos[1].codeSize = fragmentShaderData.size() / sizeof(uint32_t);
    createInfos[1].pCode = reinterpret_cast<const uint32_t*>(&fragmentShaderData.front());

    return createInfos;
}

std::vector<VkPipelineShaderStageCreateInfo> getShaderStageInfos(
	const std::vector<VkShaderModuleCreateInfo>& moduleInfos)
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages{ {}, {} };

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = &moduleInfos[0];
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].pName = "main";

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].pNext = &moduleInfos[1];
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].pName = "main";

    return shaderStages;
}

std::vector<VkVertexInputBindingDescription> getVertexBindings()
{
    std::vector<VkVertexInputBindingDescription> bindings(1);
    bindings[0].binding = 0;
    bindings[0].stride = sizeof(Vertex);
    bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindings;
}

std::vector<VkVertexInputAttributeDescription> getAttributeBindings()
{
    std::vector<VkVertexInputAttributeDescription> attributes(2);

    attributes[0].location = 0;
    attributes[0].binding = 0;
    attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributes[0].offset = offsetof(Vertex, position);
    
    attributes[1].location = 1;
    attributes[1].binding = 0;
    attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributes[1].offset = offsetof(Vertex, color);

    return attributes;
}

VkPipelineVertexInputStateCreateInfo getVertexInputInfo(
	const std::vector<VkVertexInputBindingDescription>& bindings,
	const std::vector<VkVertexInputAttributeDescription>& attributes)
{
    VkPipelineVertexInputStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.vertexBindingDescriptionCount = bindings.size();
    info.pVertexBindingDescriptions = &bindings.front();
    info.vertexAttributeDescriptionCount = attributes.size();
    info.pVertexAttributeDescriptions = &attributes.front();

    return info;
}

VkPipelineInputAssemblyStateCreateInfo getInputAssemblyInfo()
{
    VkPipelineInputAssemblyStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    return info;
}

VkViewport getViewport(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    return viewport;
}

VkRect2D getScissor(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkRect2D scissor;
    scissor.offset = { 0, 0 };
    scissor.extent = {
	static_cast<uint32_t>(width),
	static_cast<uint32_t>(height)
    };

    return scissor;
}

VkPipelineViewportStateCreateInfo getViewportInfo(
	const VkViewport& viewport,
	const VkRect2D& scissor)
{
    VkPipelineViewportStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.viewportCount = 1;
    info.pViewports = &viewport;
    info.scissorCount = 1;
    info.pScissors = &scissor;

    return info;
}

VkPipelineRasterizationStateCreateInfo getRasterizationInfo()
{
    VkPipelineRasterizationStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.polygonMode = VK_POLYGON_MODE_FILL;
    info.cullMode = VK_CULL_MODE_NONE;
    info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    
    return info;
}

VkPipelineMultisampleStateCreateInfo getMultisampleInfo()
{
    VkPipelineMultisampleStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    
    return info;
}

VkPipelineColorBlendAttachmentState getColorBlendAttachmentInfo()
{
    VkPipelineColorBlendAttachmentState info;
    info.blendEnable = VK_TRUE;
    info.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    info.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    info.colorBlendOp = VK_BLEND_OP_ADD;
    info.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    info.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    info.alphaBlendOp = VK_BLEND_OP_MAX;
    info.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
	| VK_COLOR_COMPONENT_G_BIT
	| VK_COLOR_COMPONENT_B_BIT
	| VK_COLOR_COMPONENT_A_BIT;
    
    return info;
}

VkPipelineColorBlendStateCreateInfo getColorBlendInfo(
	const VkPipelineColorBlendAttachmentState& colorBlendAttachmentInfo)
{
    VkPipelineColorBlendStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &colorBlendAttachmentInfo;

    return info;
}
