#include "RenderingManager.h"

#include "InstanceHelpers.h"
#include "SynchronizationHelpers.h"
#include "RenderPassHelpers.h"
#include "GraphicsPipelineHelpers.h"

RenderingManager::RenderingManager() :
    window(),
    instance(createRenderingInstance()),
    windowSurface(instance.getHandle(), window.getHandle()),
    device(instance.getHandle(), windowSurface.getHandle()),
    finishedRenderingSemaphore(createBinarySemaphore(device.getHandle())),
    finishedRenderingFence(createFence(device.getHandle())),
    renderPass(createRenderingRenderPass(device.getHandle())),
    swapchain(instance.getHandle(), windowSurface.getHandle(), device, renderPass.getHandle(), window.getHandle()),
    pipelineLayout(createEmptyPipelineLayout(device.getHandle())),
    graphicsPipeline(createRenderingPipeline(device.getHandle(), renderPass.getHandle(), pipelineLayout.getHandle(), window.getHandle()))
{

}
