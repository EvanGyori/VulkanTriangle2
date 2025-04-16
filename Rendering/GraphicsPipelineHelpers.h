#pragma once

#include "PipelineLayout.h"
#include "Pipeline.h"

PipelineLayout createEmptyPipelineLayout(VkDevice device);

Pipeline createRenderingPipeline(
	VkDevice device,
	VkRenderPass renderPass,
	VkPipelineLayout layout,
	GLFWwindow* window);
