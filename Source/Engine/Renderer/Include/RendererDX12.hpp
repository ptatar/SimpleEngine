#pragma once

//#include "Window/IWindow.hpp"
#include "Window/IWindowEventHandler.hpp"
#include "D3DIncludes.hpp"
#include "Common/Types.hpp"
#include "HardwareManager.hpp"
#include "ResourcesManager.hpp"
#include "SwapChain.hpp"
#include "Device.hpp"

#include <vector>
#include <memory>

namespace engine {

class Renderer: IWindowEventHandler {
public:
	Renderer();
	~Renderer();
	//Bool Initialize(IWindow*);
	void Uninitialize();
	ResourcesManager* GetResourcesManager();
	Bool MainLoop();
	Bool Update();

	// IWindowEventHandler
	void OnStart();
	void OnExit();
	void OnResize(unsigned width, unsigned height);
	void OnReposition(unsigned x, unsigned y);

private:
	ResourcesManager m_resourcesManager;
	std::shared_ptr<HardwareManager>  m_hardwareManager;
	std::unique_ptr<SwapChain>        m_swapChain;
	std::shared_ptr<Device>           m_device;
	ComPtr<ID3D12CommandQueue>        m_commandQueue;
	ComPtr<ID3D12DescriptorHeap>      m_descriptorHeap;
	ComPtr<ID3D12Resource>            m_backBufferRenderTarget;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12PipelineState>       m_pipelineState;

	HANDLE                            m_fenceEvent;
	ComPtr<IDXGIAdapter1>			  m_adapter;
	Uint64                            m_fenceValue;

	

	IWindow*                          m_window;

}; // class Renderer

} // namespace engine
