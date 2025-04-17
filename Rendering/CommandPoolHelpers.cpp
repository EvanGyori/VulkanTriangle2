#include "CommandPoolHelpers.h"

CommandPool createRenderingCommandPool(VkDevice device, uint32_t graphicsQueueFamilyIndex)
{
    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

    return CommandPool(device, createInfo);
}
