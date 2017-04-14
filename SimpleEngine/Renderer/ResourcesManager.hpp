#pragma once

namespace engine {

class ResourcesManager {
public:
	ResourcesManager();
	ResourcesManager(ResourcesManager&) = delete;
	ResourcesManager(ResourcesManager&&) = delete;
	~ResourcesManager();
};

} // namespace engine
