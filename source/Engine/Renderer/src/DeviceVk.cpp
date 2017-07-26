#include "DeviceVk.hpp"

#include "Logger.hpp"

#include <sstream>
#include <vector>

namespace engine
{
	Bool DeviceVk::Initialize()
	{
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
		instanceCreateInfo.enabledExtensionCount = 0;
		instanceCreateInfo.ppEnabledExtensionNames = nullptr;

		VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &m_instance);
		if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			LOGE("Vk instance creation failure. Incopatible driver: 0x%X", result);
			return false;
		}
		else if (result != VK_SUCCESS)
		{
			LOGE("Vk instance creation failure: 0x%X", result);
		}

		Uint32 adapterCount;
		result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, NULL);
		if (result != VK_SUCCESS)
		{
			LOGE("Vk physical devices enumeration failure: 0x%X", result);
		}

		std::vector<VkPhysicalDevice> m_adapters(adapterCount);
		result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, m_adapters.data());
		if (result != VK_SUCCESS)
		{
			LOGE("Vk adapters enumeration failure: 0x%X");
		}

		LOGI("Enumerated devices:");
		for (auto& adapter : m_adapters)
		{
			VkPhysicalDeviceProperties adapterProperties;
			vkGetPhysicalDeviceProperties(adapter, &adapterProperties);
			LOGI("\n%s", AdapterPropertiesToString(adapterProperties).c_str());
		}
		return true;
	}

	void DeviceVk::Shutdown()
	{
		vkDestroyInstance(m_instance, nullptr);
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

} // namespace engine