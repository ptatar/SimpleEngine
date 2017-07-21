#pragma once

#include "Types.hpp"

#include "vulkan/vulkan.hpp"

namespace engine
{

	class DeviceVk
	{
	public:
		DeviceVk() {};
		~DeviceVk() {};

		Bool Initialize();
	};

} // namespace engine