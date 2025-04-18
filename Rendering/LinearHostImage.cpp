#include "LinearHostImage.h"

#include <utility>
#include <stdexcept>

static VkImageCreateInfo getImageCreateInfo(GLFWwindow* window);

static VkMemoryAllocateInfo getMemoryAllocateInfo(
	VkDevice device,
	VkPhysicalDevice physicalDevice,
	VkImage image);

static uint32_t findRequiredMemoryTypeIndex(
	VkPhysicalDevice physicalDevice,
	uint32_t memoryTypesBitMask,
	VkMemoryPropertyFlags requiredProperties);

LinearHostImage::LinearHostImage(VkDevice device, VkPhysicalDevice physicalDevice, GLFWwindow* window) :
    Image(device, getImageCreateInfo(window)),
    memory(device, getMemoryAllocateInfo(device, physicalDevice, getHandle())),
    data(nullptr)
{
    // Bind the image to the newly allocated device memory
    VK_CHECK(vkBindImageMemory(device, getHandle(), memory.getHandle(), 0));

    // Map the image's memory to the host domain
    //VkMemoryRequirements requirements;
    //vkGetImageMemoryRequirements(device, getHandle(), &requirements);
    VK_CHECK(vkMapMemory(device, memory.getHandle(), 0, VK_WHOLE_SIZE, 0, &data));

    // Transfer to appropiate layout for transferring and writing
    
}

LinearHostImage::LinearHostImage(LinearHostImage&& rhs) :
    Image(std::move(rhs)),
    memory(std::move(rhs.memory)),
    data(rhs.data)
{
    rhs.data = nullptr;
}

LinearHostImage& LinearHostImage::operator=(LinearHostImage&& rhs)
{
    memory = std::move(rhs.memory);

    data = rhs.data;
    rhs.data = nullptr;

    Image::operator=(std::move(rhs));

    return *this;
}

void* LinearHostImage::getData()
{
    return data;
}

VkDeviceMemory LinearHostImage::getMemoryHandle()
{
    return memory.getHandle();
}

VkImageCreateInfo getImageCreateInfo(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    createInfo.extent.width = static_cast<uint32_t>(width);
    createInfo.extent.height = static_cast<uint32_t>(height);
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_LINEAR;
    createInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    return createInfo;
}

VkMemoryAllocateInfo getMemoryAllocateInfo(
	VkDevice device,
	VkPhysicalDevice physicalDevice,
	VkImage image)
{
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(device, image, &requirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = findRequiredMemoryTypeIndex(physicalDevice, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    return allocInfo;
}

uint32_t findRequiredMemoryTypeIndex(
	VkPhysicalDevice physicalDevice,
	uint32_t memoryTypesBitMask,
	VkMemoryPropertyFlags requiredProperties)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
	VkMemoryType memoryType = memoryProperties.memoryTypes[i];
	const uint32_t memoryTypeBits = (1 << i);
	const bool isRequiredMemoryType = memoryTypesBitMask & memoryTypeBits;

	const bool hasRequiredProperties = (requiredProperties & memoryType.propertyFlags) == requiredProperties;

	if (isRequiredMemoryType && hasRequiredProperties) {
	    return i;
	}
    }

    throw std::runtime_error("Failed to find memory type that met requirements");
    return 0;
}
