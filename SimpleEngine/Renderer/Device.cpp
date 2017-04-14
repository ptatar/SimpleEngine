#include "Device.hpp"

#include "Common/Logger.hpp"

namespace engine {

	Bool Device::Create(ComPtr<IDXGIAdapter>& adapter, bool debug)
	{
		if (debug)
		{
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController))))
			{
				LOGE("Could not create debug layer!");
				return false;
			}

			m_debugController->EnableDebugLayer();
		}

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_1;
		HRESULT result = D3D12CreateDevice(adapter.Get(),
			featureLevel,
			__uuidof(ID3D12Device1),
			reinterpret_cast<void**>(m_device.GetAddressOf()));

		if (FAILED(result))
		{
			LOGE("Could not create D3D12_1 device err: 0x%X", result);
			return false;
		}
		return true;
	}

	ComPtr<ID3D12CommandQueue> Device::CreateDirectCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDesc.NodeMask = 0;
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;

		ComPtr<ID3D12CommandQueue> commandQueue;
		HRESULT result = m_device->CreateCommandQueue( &commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));
		if (FAILED(result))
		{
			LOGE("Could not create command queue err: 0x%X", result);
			return nullptr;
		}
		return commandQueue;
	}

	ComPtr<ID3D12CommandList> Device::CreateCommandList()
	{
		return CreateCommandList(ComPtr<ID3D12CommandAllocator>(nullptr), ComPtr<ID3D12PipelineState>(nullptr));
	}

	ComPtr<ID3D12CommandList> Device::CreateCommandList(ComPtr<ID3D12CommandAllocator>& commandAllocator, ComPtr<ID3D12PipelineState>& pipelineState)
	{
		m_device->CreateCommandList()
	}

	ComPtr<ID3D12CommandAllocator> Device::CreateCommandAllocator()
	{
	ComPtr<ID3D12CommandAllocator> commandAllocator;
		HRESULT result = m_device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()));
		if (FAILED(result))
		{
			LOGE("Could not create command allocator err: 0x%X", result);
		}
		return commandAllocator;
	}

	ComPtr<ID3D12PipelineState> Device::CreatePipelineState()
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc;
		pipelineStateDesc.
		m_device->CreateGraphicsPipelineState()
	}

	ComPtr<ID3D12DescriptorHeap> Device::CreateRenderTargetDescriptorHeap(Uint32 bufferNum)
	{
		D3D12_DESCRIPTOR_HEAP_DESC renderTargetHeapDesc;
		renderTargetHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		renderTargetHeapDesc.NodeMask = 0;
		renderTargetHeapDesc.NumDescriptors = bufferNum;
		renderTargetHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		ComPtr<ID3D12DescriptorHeap> renderTargetDescriptorHeap;
		HRESULT result = m_device->CreateDescriptorHeap(
			&renderTargetHeapDesc,
			__uuidof(ID3D12DescriptorHeap),
			(void**)&renderTargetDescriptorHeap);

		if (FAILED(result))
		{
			LOGE("Could not create RT heap descriptor! err: 0x%X", result);
			return nullptr;
		}
		return renderTargetDescriptorHeap;
	}

	ComPtr<ID3D12DescriptorHeap> Device::CreateDepthStencilDescriptorHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC depthStencilHeapDesc;
		depthStencilHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		depthStencilHeapDesc.NodeMask = 0;
		depthStencilHeapDesc.NumDescriptors = 1;
		depthStencilHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
		HRESULT result = m_device->CreateDescriptorHeap(
			&depthStencilHeapDesc,
			__uuidof(ID3D12DescriptorHeap),
			(void**)&depthStencilDescriptorHeap);

		if (FAILED(result))
		{
			LOGE("Could not create RT heap descriptor! err: 0x%X", result);
			return nullptr;
		}
		return depthStencilDescriptorHeap;
	}

	Uint32 Device::GetRenderTargetDescriptorSize()
	{
		return m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	Uint32 Device::GetDepthStencilDescriptorSize()
	{
		return m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

} // namespace engine