#include "RenderingManager.h"

#include <limits>

#include "InstanceHelpers.h"
#include "DebugUtilsMessengerHelpers.h"
#include "SynchronizationHelpers.h"
#include "RenderPassHelpers.h"
#include "GraphicsPipelineHelpers.h"
#include "CommandPoolHelpers.h"
#include "EnumerationHelpers.h"

RenderingManager::RenderingManager() :
    window(),
    instance(createRenderingInstance()),
    debugMessenger(createDebugger(instance.getHandle())),
    windowSurface(instance.getHandle(), window.getHandle()),
    device(instance.getHandle(), windowSurface.getHandle()),
    acquiredImageSemaphore(createBinarySemaphore(device.getHandle())),
    finishedRenderingSemaphore(createBinarySemaphore(device.getHandle())),
    finishedRenderingFence(createFence(device.getHandle())),
    renderPass(createRenderingRenderPass(device.getHandle())),
    swapchain(instance.getHandle(), windowSurface.getHandle(), device, renderPass.getHandle(), window.getHandle()),
    pipelineLayout(createEmptyPipelineLayout(device.getHandle())),
    graphicsPipeline(createRenderingPipeline(device.getHandle(), renderPass.getHandle(), pipelineLayout.getHandle(), window.getHandle())),
    commandPool(createRenderingCommandPool(device.getHandle(), device.getGraphicsQueueFamily().queueFamilyIndex)),
    commandBuffer(VK_NULL_HANDLE)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.getHandle();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VK_CHECK(vkAllocateCommandBuffers(device.getHandle(), &allocInfo, &commandBuffer));
}

RenderingManager::~RenderingManager()
{
    VK_CHECK(vkDeviceWaitIdle(device.getHandle()));
}

bool RenderingManager::shouldLoop()
{
    return glfwWindowShouldClose(window.getHandle()) == 0;
}

void RenderingManager::draw(const std::vector<Vertex>& buffer)
{
    glfwPollEvents();

    VkFence fence = finishedRenderingFence.getHandle();
    vkWaitForFences(device.getHandle(), 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
    vkResetFences(device.getHandle(), 1, &fence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device.getHandle(), swapchain.getHandle(), std::numeric_limits<uint64_t>::max(), acquiredImageSemaphore.getHandle(), VK_NULL_HANDLE, &imageIndex);

    VK_CHECK(vkResetCommandPool(device.getHandle(), commandPool.getHandle(), 0));
    recordCommandBuffer(buffer, imageIndex);

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

    VkSemaphore acquiredImageSemaphoreHandle = acquiredImageSemaphore.getHandle();
    VkSemaphore finishedRenderingSemaphoreHandle = finishedRenderingSemaphore.getHandle();

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &acquiredImageSemaphoreHandle;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &finishedRenderingSemaphoreHandle;

    VK_CHECK(vkQueueSubmit(device.getGraphicsQueueFamily().queues[0], 1, &submitInfo, finishedRenderingFence.getHandle()));

    VkSwapchainKHR swapchainHandle = swapchain.getHandle();

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &finishedRenderingSemaphoreHandle;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchainHandle;
    presentInfo.pImageIndices = &imageIndex;

    VK_CHECK(vkQueuePresentKHR(device.getPresentQueueFamily().queues[0], &presentInfo));
}

VkRect2D RenderingManager::getRenderArea()
{
    int width, height;
    glfwGetFramebufferSize(window.getHandle(), &width, &height);

    VkRect2D area = {
	{ 0, 0 },
	{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }
    };

    return area;
}

void RenderingManager::recordCommandBuffer(const std::vector<Vertex>& buffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    // Sets all color values to 0, which gives black
    VkClearValue clearValue = {};

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass.getHandle();
    renderPassInfo.framebuffer = swapchain.getFramebuffer(imageIndex).getHandle();
    renderPassInfo.renderArea = getRenderArea();
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.getHandle());

    // TODO bind a vertex buffer

    /*
    VkClearAttachment clearAttachment = {};
    clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clearAttachment.colorAttachment = 0;
    clearAttachment.clearValue.color.float32[0] = 1.0f;

    int width, height;
    glfwGetFramebufferSize(window.getHandle(), &width, &height);

    VkClearRect clearRect = {};
    clearRect.rect = { { static_cast<int32_t>(width) / 2, static_cast<int32_t>(height) / 2 }, { 200, 200 } };
    clearRect.layerCount = 1;

    vkCmdClearAttachments(commandBuffer, 1, &clearAttachment, 1, &clearRect);
    */

    vkCmdDraw(commandBuffer, 6, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    /*
    static float val = 0.0f;
    val += 0.01f;
    if (val > 1.0f) {
	val = 0.0f;
    }

    VkClearColorValue clearValue = {};
    clearValue.float32[1] = val;

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    auto images = getSwapchainImagesKHR(device.getHandle(), swapchain.getHandle());

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.image = images[imageIndex];
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // Perform an image layout transition to the necessary layout for clearing. The clear operation from the previous frame is available since this waits on the fence which waits till it is available. However, it is not visible. So use the memory barrier to make it visible to the clear operation. Also have the clear operation wait till the image layout transition has been completed and made visible.
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
	    0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

    vkCmdClearColorImage(commandBuffer, images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearValue, 1, &subresourceRange);

    imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = 0; // visibility operation automatically performed by VkQueuePresentKHR. Availability operation also performed since VkQueuePresentKHR waits on the semaphore signaled by completion of this command buffer
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.image = images[imageIndex];
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // Just perform the necessary image layout transition to the present src layout. Wait on the memory writes performed by the clear for the transition but have no memory accesses wait on it.
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
	    0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
    */

    VK_CHECK(vkEndCommandBuffer(commandBuffer));
}
