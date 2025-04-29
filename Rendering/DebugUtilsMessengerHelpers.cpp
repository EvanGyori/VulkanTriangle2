#include "DebugUtilsMessengerHelpers.h"

#include <string>
#include <iostream>

const char* getMessageSeverityString(VkDebugUtilsMessageSeverityFlagBitsEXT severityFlag);

std::string getMessageTypesString(VkDebugUtilsMessageTypeFlagsEXT typeFlags);

VkBool32 VKAPI_PTR debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
	void*                                            pUserData)
{
    std::cout << "VULKAN: " << getMessageSeverityString(messageSeverity)
	<< " TYPES: " << getMessageTypesString(messageTypes)
	<< " MESSAGE: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT getDebuggerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
	//| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
	| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
	| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;

    return createInfo;
}

DebugUtilsMessenger createDebugger(VkInstance instance)
{
    auto createInfo = getDebuggerCreateInfo();

    return DebugUtilsMessenger(instance, createInfo);
}

const char* getMessageSeverityString(VkDebugUtilsMessageSeverityFlagBitsEXT severityFlag)
{
    switch (severityFlag) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	    return "verbose";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
	    return "info";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
	    return "warning";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
	    return "error";
	default:
	    return "unknownSeverity";
    }
}

std::string getMessageTypesString(VkDebugUtilsMessageTypeFlagsEXT typeFlags)
{
    std::string types = "";

    if (typeFlags & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
	types = "general";
    }

    if (typeFlags & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
	if (types != "") {
	    types += ", ";
	}
	types += "validation";
    }

    if (typeFlags & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
	if (types != "") {
	    types += ", ";
	}
	types += "performance";
    }

    return (types == "") ? "unknownType" : types;
}
