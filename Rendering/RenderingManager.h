#pragma once

#include <vector>

#include "Window.h"
#include "Instance.h"
#include "Surface.h"
#include "RenderingDevice.h"
#include "Semaphore.h"
#include "Fence.h"
#include "RenderPass.h"
#include "RenderingSwapchain.h"
#include "PipelineLayout.h"
#include "Pipeline.h"
#include "CommandPool.h"
#include "Vertex.h"

class RenderingManager
{
public:
    RenderingManager();

    ~RenderingManager();

    /*
     * Returns true if the program should continue running.
     * False if the window is attempting to close
     */
    bool shouldLoop();

    // For every three vertices, a triangle is formed. counter-clockwise for front facing triangles
    void draw(const std::vector<Vertex>& buffer);

private:
    Window window;
    Instance instance;
    Surface windowSurface;
    RenderingDevice device;
    Semaphore acquiredImageSemaphore, finishedRenderingSemaphore;
    Fence finishedRenderingFence;
    RenderPass renderPass;
    RenderingSwapchain swapchain;
    PipelineLayout pipelineLayout;
    Pipeline graphicsPipeline;
    CommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkRect2D getRenderArea();

    void recordCommandBuffer(const std::vector<Vertex>& buffer, uint32_t imageIndex);
};
