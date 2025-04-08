#include "InstanceHelpers.h"

#include <vector>
#include <iostream>
#include <cstring>
#include <stdexcept>

#include "EnumerationHelpers.h"
#include "Utility.h"

std::vector<const char*> getRequiredExtensions();

std::vector<const char*> getRequiredLayers();

bool areExtensionsSupported(const std::vector<const char*>& extensions);

bool areLayersSupported(const std::vector<const char*>& layers);

VkApplicationInfo getApplicationInfo();

Instance createRenderingInstance()
{
    auto extensions = getRequiredExtensions();
    auto layers = getRequiredLayers();

    if (!areExtensionsSupported(extensions)) {
	throw std::runtime_error("Required instance extensions are not supported");
    }

    if (!areLayersSupported(layers)) {
	throw std::runtime_error("Required instance layers are not supported");
    }

    VkApplicationInfo appInfo = getApplicationInfo();

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = &layers.front();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = &extensions.front();

    return Instance(createInfo);
}

std::vector<const char*> getRequiredExtensions()
{
    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

std::vector<const char*> getRequiredLayers()
{
    return { "VK_LAYER_KHRONOS_validation" };
}

bool areExtensionsSupported(const std::vector<const char*>& extensions)
{
    std::vector<VkExtensionProperties> supportedExtensions = enumerateInstanceExtensionProperties(nullptr);
    std::vector<const char*> supportedExtensionNames(supportedExtensions.size());
    for (int i = 0; i < supportedExtensions.size(); ++i) {
	supportedExtensionNames[i] = supportedExtensions[i].extensionName;
    }

    return isASubsetOfB(extensions, supportedExtensionNames);
}

bool areLayersSupported(const std::vector<const char*>& layers)
{
    std::vector<VkLayerProperties> supportedLayers = enumerateInstanceLayerProperties();
    std::vector<const char*> supportedLayerNames(supportedLayers.size());
    for (int i = 0; i < supportedLayers.size(); ++i) {
	supportedLayerNames[i] = supportedLayers[i].layerName;
    }

    return isASubsetOfB(layers, supportedLayerNames);
}

VkApplicationInfo getApplicationInfo()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_4;
    return appInfo;
}
