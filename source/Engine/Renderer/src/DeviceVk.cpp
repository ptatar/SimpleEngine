#include "DeviceVk.hpp"

#include "Logger.hpp"

#include <sstream>
#include <vector>
#include <cstring>

namespace engine
{
    Bool DeviceVk::Initialize()
    {
		std::vector<char*> requiredExtension = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
		Uint32 extensionCount;
		VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		if (result != VK_SUCCESS)
		{
			LOGE("Extension enumeration failure: %d", result);
			return false;
		}

		std::vector<VkExtensionProperties> availableExtension(extensionCount);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtension.data());
		if (result != VK_SUCCESS)
		{
			LOGE("Extension enumeration failure: %d", result);
			return false;
		}

		std::vector<Bool> extensionCheckResult(requiredExtension.size(), false);
		Bool extensionSupportError = false;
		for (Uint32 i = 0; i < requiredExtension.size(); ++i)
		{
			Bool supported = false;
			char* required = requiredExtension[i];
			for (auto& available : availableExtension)
			{
				if (std::strcmp(required,available.extensionName) == 0)
				{
					supported = true;
					break;
				}
			}
			if (supported == false)
			{
				LOGE("Required extension not supported: %s", requiredExtension[i]);
				extensionSupportError = true;
			}
		}

		if (extensionSupportError)
		{
			return false;
		}
		
        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "VkSimpleApp";
        applicationInfo.applicationVersion = 0;
        applicationInfo.pEngineName = "SimpleEngine";
        applicationInfo.engineVersion = 0;
        applicationInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledExtensionCount = requiredExtension.size();
        instanceCreateInfo.ppEnabledExtensionNames = requiredExtension.data();

        result = vkCreateInstance(&instanceCreateInfo, NULL, &m_instance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
        {
            LOGE("Vk instance creation failure. Incopatible driver: %d", result);
            return false;
        }
        else if (result != VK_SUCCESS)
        {
            LOGE("Vk instance creation failure: %d", result);
			return false;
        }

        Uint32 adapterCount;
        result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, NULL);
        if (result != VK_SUCCESS)
        {
            LOGE("Vk physical devices enumeration failure: %d", result);
			return false;
        }

        std::vector<VkPhysicalDevice> m_adapters(adapterCount);
        result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, m_adapters.data());
        if (result != VK_SUCCESS)
        {
            LOGE("Vk adapters enumeration failure: %d", result);
			return false;
        }

        LOGI("Enumerated adapters:");
        Uint32 selectedAdapterIndex = adapterCount;
        for (Uint32 i = 0; i < m_adapters.size(); ++i)
        {
            VkPhysicalDeviceProperties adapterProperties;
            vkGetPhysicalDeviceProperties(m_adapters[i], &adapterProperties);
            LOGI("\n%s", AdapterPropertiesToString(adapterProperties).c_str());
            if (adapterProperties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
                adapterProperties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
				LOGI("Adapter selected: \n%s", AdapterPropertiesToString(adapterProperties).c_str());
                selectedAdapterIndex = i;
                break;
            }
        }
		if (selectedAdapterIndex >= adapterCount)
		{
			LOGE("Adapter selection failure");
			return false;
		}
		
		Uint32 queuePropertiesCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_adapters[selectedAdapterIndex], &queuePropertiesCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queuePropertiesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_adapters[selectedAdapterIndex], &queuePropertiesCount, queueFamilyProperties.data());

		LOGI("Listing queue families:");
		Uint32 queueFamilyIndex = queuePropertiesCount;
		for (Uint32 i = 0; i < queueFamilyProperties.size(); ++i)
		{
			LOGI("Queue Family %d\n%s", i, QueueFamilyToString(queueFamilyProperties[i]).c_str());
			if (queueFamilyProperties[i].queueCount > 0 &&
				queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT &&
				queueFamilyIndex == queuePropertiesCount)
			{
				queueFamilyIndex = i;
			}

		}

		if (queueFamilyIndex >= queuePropertiesCount)
		{
			LOGE("Adapter queues don't fulfil minimal requirements");
			return false;
		}
		LOGI("Selected queue family index %d", queueFamilyIndex);
		
		Float queuePriorities = 1.0f;
		VkDeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.pNext = nullptr;
		queueCreateInfo.flags = 0;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriorities;

		VkDeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = nullptr;
		deviceCreateInfo.flags = 0;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = nullptr;
		deviceCreateInfo.pEnabledFeatures = nullptr;

		result = vkCreateDevice(m_adapters[selectedAdapterIndex], &deviceCreateInfo, NULL, &m_device);
		if (result != VK_SUCCESS)
		{
			LOGE("Device creation failure: %d", result);
			return false;
		}

		VkCommandPoolCreateInfo commandPoolCreateInfo;
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.pNext = nullptr;
		commandPoolCreateInfo.flags = 0;
		commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

		result = vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool);
		if (result != VK_SUCCESS)
		{
			LOGE("Command pool creation failure: %d", result);
			return false;
		}

		VkCommandBufferAllocateInfo commandBufferAllocateInfo;
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.pNext = nullptr;
		commandBufferAllocateInfo.commandPool = m_commandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = 1;

		result = vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &m_commandBuffer);
		if (result != VK_SUCCESS)
		{
			LOGE("Command buffer allocation failure: %d", result);
			return false;
		}

        return true;
    }

    void DeviceVk::Shutdown()
    {
		vkDestroyCommandPool(m_device, m_commandPool, nullptr);
		vkDestroyDevice(m_device, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

	engine::Bool DeviceVk::CreateSurface(HINSTANCE hInstance, HWND hwnd)
	{
		VkSurfaceKHR surface;
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.pNext = nullptr;
		surfaceCreateInfo.flags = 0;
		surfaceCreateInfo.hinstance = hInstance;
		surfaceCreateInfo.hwnd = hwnd;
		VkResult result = vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &surface);
		if (result != VK_SUCCESS)
		{
			LOGE("Surface creation failure: %d", result);
			return
		}

	}

	std::string DeviceVk::AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const
    {
        std::stringstream buf;
        buf << "Name:        " << adapterProperties.deviceName << "\n"
            << "Vendor:      " << adapterProperties.vendorID << "\n"
            << "Api version: " << adapterProperties.apiVersion << "\n"
            << "Device ID:   " << adapterProperties.deviceID << "\n"
            << "Device Type: " << AdapterTypeToString(adapterProperties.deviceType);

        return buf.str();
    }

    std::string DeviceVk::AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const
    {
        switch (adpaterType)
        {
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
                return "CPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return "DISCRETE_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return "INTEGRETED_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return "VIRTUAL_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
                return "OTHER";
            default:
                return "UNKNOWN";
        }
    }

	std::string DeviceVk::QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const
	{
		std::stringstream buf;
		buf << "Queues count:                          " << queueFamily.queueCount << "\n"
			<< "Queue flag:                            "
			<< (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ? "GRAPHICS_BIT " : "")
			<< (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT ? "COMPUTE_BIT " : "")
			<< (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT ? "TRANSFER_BIT " : "")
			<< (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT ? "SPARSE_BINDING_BIT" : "") << "\n"
			<< "Queue timestamp valid bits:            " << queueFamily.timestampValidBits << "\n"
			<< "Queue min transfer granularity (w/h/d):" << queueFamily.minImageTransferGranularity.width << " "
			<< queueFamily.minImageTransferGranularity.height << " "
			<< queueFamily.minImageTransferGranularity.depth << " ";
		return buf.str();
	}

} // namespace engine
