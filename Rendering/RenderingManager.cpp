#include "RenderingManager.h"

#include <limits>

#include "InstanceHelpers.h"
#include "DebugUtilsMessengerHelpers.h"
#include "SynchronizationHelpers.h"
#include "RenderPassHelpers.h"
#include "GraphicsPipelineHelpers.h"
#include "CommandPoolHelpers.h"
#include "EnumerationHelpers.h"

static int getTexelOffset(VkSubresourceLayout layout, int elementSize, int x, int y, int z, int layer);

RenderingManager::RenderingManager() :
    window(),
    instance(createRenderingInstance()),
    debugMessenger(createDebugger(instance.getHandle())),
    windowSurface(instance.getHandle(), window.getHandle()),
    device(instance.getHandle(), windowSurface.getHandle()),
    acquiredImageSemaphore(createBinarySemaphore(device.getHandle())),
    finishedRenderingSemaphore(createBinarySemaphore(device.getHandle())),
    finishedRenderingFence(createFence(device.getHandle(), true)),
    renderPass(createRenderingRenderPass(device.getHandle())),
    swapchain(instance.getHandle(), windowSurface.getHandle(), device, renderPass.getHandle(), window.getHandle()),
    pipelineLayout(createEmptyPipelineLayout(device.getHandle())),
    graphicsPipeline(createRenderingPipeline(device.getHandle(), renderPass.getHandle(), pipelineLayout.getHandle(), window.getHandle())),
    commandPool(createRenderingCommandPool(device.getHandle(), device.getGraphicsQueueFamily().queueFamilyIndex)),
    commandBuffer(VK_NULL_HANDLE),
    image(device.getHandle(), device.getPhysicalDevice(), window.getHandle())
{
    // allocate drawing command buffer
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.getHandle();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VK_CHECK(vkAllocateCommandBuffers(device.getHandle(), &allocInfo, &commandBuffer));

    // Transition image layout to general
    VkCommandBuffer transitionCommandBuffer;
    VK_CHECK(vkAllocateCommandBuffers(device.getHandle(), &allocInfo, &transitionCommandBuffer));

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(transitionCommandBuffer, &beginInfo));

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_HOST_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.image = image.getHandle();
    barrier.subresourceRange = subresourceRange;

    vkCmdPipelineBarrier(transitionCommandBuffer,
	    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_HOST_BIT,
	    0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    VK_CHECK(vkEndCommandBuffer(transitionCommandBuffer));

    Fence transitionFence = createFence(device.getHandle());

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &transitionCommandBuffer;


    VK_CHECK(vkQueueSubmit(device.getGraphicsQueueFamily().queues[0], 1, &submitInfo, transitionFence.getHandle()));

    VkFence fenceHandle = transitionFence.getHandle();
    VK_CHECK(vkWaitForFences(device.getHandle(), 1, &fenceHandle, VK_TRUE, std::numeric_limits<uint64_t>::max()));
    
    vkFreeCommandBuffers(device.getHandle(), commandPool.getHandle(), 1, &transitionCommandBuffer);

    VkMappedMemoryRange memoryRange = {};
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.memory = image.getMemoryHandle();
    memoryRange.size = VK_WHOLE_SIZE;

    VK_CHECK(vkInvalidateMappedMemoryRanges(device.getHandle(), 1, &memoryRange));
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

    // Temp - write to linear host image before copying it over
    writeImage();

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
    int width, height;
    glfwGetFramebufferSize(window.getHandle(), &width, &height);

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

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);


    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    auto images = getSwapchainImagesKHR(device.getHandle(), swapchain.getHandle());

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; // renderpass converts to this layout
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.image = images[imageIndex];
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // Perform an image layout transition to the necessary layout for clearing. The clear operation from the previous frame is available since this waits on the fence which waits till it is available. However, it is not visible. So use the memory barrier to make it visible to the clear operation. Also have the clear operation wait till the image layout transition has been completed and made visible.
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
	    0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
    /*

    static float val = 0.0f;
    val += 0.01f;
    if (val > 1.0f) {
	val = 0.0f;
    }

    VkClearColorValue clearValue = {};
    clearValue.float32[1] = val;

    vkCmdClearColorImage(commandBuffer, images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearValue, 1, &subresourceRange);
    */

    VkImageCopy region = {};
    region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    region.dstSubresource = region.srcSubresource;
    region.extent.width = 100;
    region.extent.height = 100;
    region.extent.depth = 1;

    vkCmdCopyImage(commandBuffer, image.getHandle(), VK_IMAGE_LAYOUT_GENERAL,
	    images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	    1, &region);

    /*
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


    imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
	| VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.image = images[imageIndex];
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // Just perform the necessary image layout transition to the present src layout. Wait on the memory writes performed by the clear for the transition but have no memory accesses wait on it.
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	    0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier); 


    

    VK_CHECK(vkEndCommandBuffer(commandBuffer));
}

void RenderingManager::writeImage()
{
    VkSubresourceLayout layout;

    VkImageSubresource subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel = 0;
    subresource.arrayLayer = 0;

    vkGetImageSubresourceLayout(device.getHandle(), image.getHandle(), &subresource, &layout);

    static char brightness = 122;
    //brightness = (brightness + 3) % 255;

    // format is VK_FORMAT_R8G8B8A8_SRGB
    char* data = reinterpret_cast<char*>(image.getData());
    for (int x = 0; x < 200; x += 1) {
	for (int y = 0; y < 200; y += 1) {
	    for (int i = 0; i < 1; i++) {
		data[getTexelOffset(layout, 4, x, y, 0, 0) + i] = brightness;
	    }
	}
    }

    VkMappedMemoryRange memoryRange = {};
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.memory = image.getMemoryHandle();
    memoryRange.size = VK_WHOLE_SIZE;

    VK_CHECK(vkFlushMappedMemoryRanges(device.getHandle(), 1, &memoryRange));
}

int getTexelOffset(VkSubresourceLayout layout, int elementSize, int x, int y, int z, int layer)
{
    return layer * layout.arrayPitch + z * layout.depthPitch + y * layout.rowPitch + x * elementSize;
}
