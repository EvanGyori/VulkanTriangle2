#pragma once

#include "Semaphore.h"
#include "Fence.h"

Semaphore createBinarySemaphore(VkDevice device);

Fence createFence(VkDevice device);
