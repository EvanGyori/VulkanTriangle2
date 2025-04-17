#pragma once

#include "DebugUtilsMessenger.h"

VkDebugUtilsMessengerCreateInfoEXT getDebuggerCreateInfo();

DebugUtilsMessenger createDebugger(VkInstance instance);
