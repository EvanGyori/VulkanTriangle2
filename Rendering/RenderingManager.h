#pragma once

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

class RenderingManager
{
public:
    RenderingManager();

private:
    Window window;
    Instance instance;
    Surface windowSurface;
    RenderingDevice device;
    Semaphore finishedRenderingSemaphore;
    Fence finishedRenderingFence;
    RenderPass renderPass;
    RenderingSwapchain swapchain;
    PipelineLayout pipelineLayout;
    Pipeline graphicsPipeline;
};
