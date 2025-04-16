#pragma once

#include "CommandPool.h"

CommandPool createRenderingCommandPool(VkDevice device, uint32_t graphicsQueueFamilyIndex);
