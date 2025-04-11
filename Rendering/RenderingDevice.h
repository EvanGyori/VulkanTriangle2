#pragma once

#include "LogicalDevice.h"

class RenderingDevice : public LogicalDevice
{
public:
    RenderingDevice(VkInstance instance, VkSurfaceKHR surface);

    RenderingDevice(RenderingDevice&& rhs);
    RenderingDevice& operator=(RenderingDevice&& rhs);

    VkQueue getGraphicsQueue();
    VkQueue getPresentQueue();

private:
    // indices within the queueFamilies array in LogicalDevice
    size_t graphicsFamilyIndex, presentFamilyIndex;
};
