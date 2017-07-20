#pragma once

#include "Types.hpp"

#include "Vulkan/"

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