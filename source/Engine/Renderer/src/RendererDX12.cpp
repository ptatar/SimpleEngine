#include "RendererDX12.hpp"

#include "D3dIncludes.hpp"
#include "HardwareManager.hpp"
#include "Common/Logger.hpp"

#include <vector>
#include <memory>

namespace engine {

	Renderer::Renderer()
		: m_fenceValue(0) {
	}


	Renderer::~Renderer()
	{}


	Bool Renderer::Initialize(IWindow* window)
	{
		HRESULT result;

		m_window = window;
		ComPtr<IDXGIFactory4> factory;
		result = CreateDXGIFactory(__uuidof(IDXGIFactory4),
			reinterpret_cast<void**>(factory.GetAddressOf()));
		if (FAILED(result))
		{
			LOGE("Could not create DXGIFactory");
			Uninitialize();
			return false;
		}


		// First adapter first display
		m_hardwareManager = std::make_shared<HardwareManager>(factory);
		if (!m_hardwareManager->Initialize())
		{
			return false;
		}

		m_device = std::make_shared<Device>();
		if (m_device->Create(m_hardwareManager->GetCurrentAdapter(), true))
		{
			return false;
		}

		m_commandQueue = m_device->CreateDirectCommandQueue();
		if (!m_commandQueue)
		{
			return false;
		}

		m_swapChain = std::make_unique<SwapChain>(factory, m_hardwareManager, m_device);
		if (!m_swapChain->Create(m_commandQueue))
		{
			return false;
		}

		LOGI("D3D12 device created");
		return true;
	}


	void Renderer::Uninitialize()
	{
	}


	ResourcesManager* Renderer::GetResourcesManager()
	{
		return nullptr;
	}


	Bool Renderer::MainLoop()
	{
		return true;
	}


	Bool Renderer::Update()
	{
		return true;
	}


	// IWindowEventHandler
	void Renderer::OnStart()
	{

	}


	void Renderer::OnExit()
	{

	}


	void Renderer::OnResize(unsigned width, unsigned height)
	{

	}


	void Renderer::OnReposition(unsigned x, unsigned y)
	{

	}

} // namespace engine
