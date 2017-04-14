#pragma once

namespace engine {

class IWindowEventHandler {
public:
	virtual ~IWindowEventHandler() {};

	virtual void OnStart() = 0;
	virtual void OnExit() = 0;
	virtual void OnResize(unsigned width, unsigned height) = 0;
	virtual void OnReposition(unsigned x, unsigned y) = 0;
};

} // namespace engine