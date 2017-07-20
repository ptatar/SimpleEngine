#include "SwapChainDX12.hpp"

#include "Common/Logger.hpp"
#include "Device.hpp"
#include "HardwareManager.hpp"

namespace engine {

Bool SwapChain::Create(ComPtr<ID3D12CommandQueue>& commandQueue) {
	DXGI_MODE_DESC displayMode = m_hardwareManager->GetCurrentDisplayMode();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	swapChainDesc.Width = displayMode.Width;
	swapChainDesc.Height = displayMode.Height;
	swapChainDesc.BufferCount = m_swapChainBuffersNum;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	HRESULT result;
	ComPtr<IDXGISwapChain1> swapChain;
	if (m_window->IsFullscreen()) {
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc;
		swapChainFullScreenDesc.RefreshRate = displayMode.RefreshRate;
		swapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainFullScreenDesc.Windowed = true;

		// NOTE I should not use this method in WindowsAP store
		result = m_factory->CreateSwapChainForHwnd(
			commandQueue.Get(),
			(HWND)(m_window->GetWindowId()),
			&swapChainDesc,
			&swapChainFullScreenDesc,
			NULL,
			&swapChain);
	}
	else {
		result = m_factory->CreateSwapChainForHwnd(
			commandQueue.Get(),
			(HWND)m_window->GetWindowId(),
			&swapChainDesc,
			NULL,
			NULL,
			&swapChain);
	}

	if (FAILED(result)) {
		LOGE("Could not create swap chain err: 0x%X", result);
		return false;
	}

	m_currentBufferIdx = 0;
	m_renderTargetDescriptorHeap = m_device->CreateRenderTargetDescriptorHeap(2);
	if (!m_renderTargetDescriptorHeap)
	{
		return false;
	}

	m_renderTargetDescriptorSize = m_device->GetRenderTargetDescriptorSize();

	m_depthStencilDescriptorHeap = m_device->CreateDepthStencilDescriptorHeap();
	if (!m_depthStencilDescriptorHeap)
	{
		return false;
	}

	m_depthStencilDescriptorSize = m_device->GetDepthStencilDescriptorSize();

	return true;
}

} // namespace engine