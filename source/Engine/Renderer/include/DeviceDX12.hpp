#pragma once

#include "D3DIncludes.hpp"
#include "Common/Types.hpp"
namespace engine
{

	class Device
	{
	public:
		Device() {}
		~Device() {}
		Bool Create(ComPtr<IDXGIAdapter>& adapter, bool debug);

		// One GPU
		ComPtr<ID3D12CommandQueue> CreateDirectCommandQueue();
		ComPtr<ID3D12CommandList> CreateCommandList();
		ComPtr<ID3D12CommandList> CreateCommandList(ComPtr<ID3D12CommandAllocator>& commandAllocator, ComPtr<ID3D12PipelineState>& pipelineState);
		ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
		ComPtr<ID3D12PipelineState> CreatePipelineState();

		ComPtr<ID3D12DescriptorHeap> CreateRenderTargetDescriptorHeap(Uint32 bufferNum);
		ComPtr<ID3D12DescriptorHeap> CreateDepthStencilDescriptorHeap();
		Uint32 GetRenderTargetDescriptorSize();
		Uint32 GetDepthStencilDescriptorSize();

	private:
		ComPtr<ID3D12Debug> m_debugController;
		ComPtr<ID3D12Device1> m_device;
	};

}