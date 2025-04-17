#include "CommandPoolHelpers.h"

#include <iostream>

CommandPool createRenderingCommandPool(VkDevice device, uint32_t graphicsQueueFamilyIndex)
{
    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

    std::cout << "command\n";

    return CommandPool(device, createInfo);
}
