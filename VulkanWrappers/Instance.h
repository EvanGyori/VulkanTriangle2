/*
 * Resource wrapper for VkInstance
 */

#pragma once

#include "VulkanWithGLFW.h"

class Instance
{
public:
    Instance(const VkInstanceCreateInfo& createInfo);

    ~Instance();

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    Instance(Instance&& rhs);
    Instance& operator=(Instance&& rhs);

    VkInstance getHandle();

private:
    VkInstance handle;
};
