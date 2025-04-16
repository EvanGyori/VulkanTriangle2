#pragma once

#include "Pipeline.h"

Pipeline createRenderingPipeline(
	VkDevice device,
	VkRenderPass renderPass,
	VkPipelineLayout layout,
	GLFWwindow* window);
