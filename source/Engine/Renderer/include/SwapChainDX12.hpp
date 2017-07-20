#include <Types.hpp>
#include <memory>

#include "D3DIncludes.hpp"
#include "Window/IWindow.hpp"


namespace engine {

class HardwareManager;
class Device;

class SwapChain {
public:
	SwapChain(ComPtr<IDXGIFactory4>& factory, std::shared_ptr<HardwareManager>& hardwareManager, std::shared_ptr<Device>& device)
		: m_factory(factory)
		, m_hardwareManager(hardwareManager)
		, m_device(device)
		, m_swapChainBuffersNum(2)
		, m_currentBufferIdx(0) {}
	~SwapChain() {}

	// TODO adapter and output depends from window so there should be some kind
	// connection between both classes
	Bool Create(ComPtr<ID3D12CommandQueue>& commandQueue);

private:
	std::shared_ptr<HardwareManager>  m_hardwareManager;
	std::shared_ptr<Device>           m_device;
	ComPtr<IDXGIFactory4>             m_factory;
	IWindow*                          m_window;
	ComPtr<ID3D12Fence>               m_fence;
	ComPtr<ID3D12DescriptorHeap>      m_renderTargetDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>      m_depthStencilDescriptorHeap;
	Uint32                            m_renderTargetDescriptorSize;
	Uint32                            m_depthStencilDescriptorSize;
	Uint32                            m_swapChainBuffersNum;
	Uint32                            m_currentBufferIdx;
};

} // namespace engine