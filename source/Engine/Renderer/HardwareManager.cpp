#include "HardwareManager.hpp"

#include "Common/Logger.hpp"

#include <memory>

namespace engine {

	Bool HardwareManager::Initialize()
	{
		// TODO This function is fairly dumb first adapter output and mode
		AdapterVector_t adaptersList;
		GetAdapters(adaptersList);
		if (!adaptersList.size())
		{
			LOGE("Could not find adapter supporting DirectX12!");
			return false;
		}

		OutputVector_t outputsList;
		GetOutputs(adaptersList[0], outputsList);
		if (!outputsList.size())
		{
			LOGE("Could not find adapter outputs!");
			return false;
		}

		DisplayModeList_t displayModesList;
		GetDisplayModes(outputsList[0], displayModesList);
		if (!displayModesList.size())
		{
			LOGE("Could not query displayModes for adapter output");
			return false;
		}

		ChooseDisplayMode();

		m_currentAdapter = adaptersList[0];
		m_currentOutput = outputsList[0];
		m_currentDisplayMode = displayModesList[0];
	}

	void HardwareManager::GetAdapters(AdapterVector_t& outAdapters) const
	{
		ComPtr<IDXGIAdapter> adapter = nullptr;
		for (unsigned adapterIndex = 0; m_factory->EnumAdapters(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
			std::unique_ptr<DXGI_ADAPTER_DESC> adapterDesc(new DXGI_ADAPTER_DESC);
			if (SUCCEEDED(adapter->GetDesc(adapterDesc.get())))
			{
				LOGI("Enumerated adapter: %ls", adapterDesc->Description);
			}

			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr)))
			{
				outAdapters.push_back(std::move(adapter));
				LOGI("Enumerated adapter: %ls supports D3D12_1", adapterDesc->Description);
			}
			else
			{
				LOGI("Enumerated adapter: %ls D3D12_1 not supported", adapterDesc->Description);
			}
		}
	}


	void HardwareManager::GetOutputs(ComPtr<IDXGIAdapter>& adapter, OutputVector_t& outAdapterOutputs) const
	{
		// there is no EnumOutputs that takes IDXGIOutput1 ??
		OutputVector_t adapterOutputs;
		ComPtr<IDXGIOutput> output;
		unsigned outputIdx = 0;
		while (SUCCEEDED(adapter->EnumOutputs(outputIdx++, &output)))
		{
			outAdapterOutputs.push_back(std::move(output));
		}
	}


	Bool HardwareManager::GetDisplayModes(ComPtr<IDXGIOutput>& adapterOutput, DisplayModeList_t& outDisplayModes) const
	{
		unsigned numModes;
		HRESULT result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes, NULL);

		if (FAILED(result))
		{
			LOGE("Could not get display modes number");
			return false;
		}

		outDisplayModes.resize(numModes);

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, outDisplayModes.data());
		if (FAILED(result))
		{
			LOGE("Could not get display modes");
			outDisplayModes.resize(0);
			return false;
		}
	}

	Bool HardwareManager::ChooseDisplayMode(const DisplayModeList_t& displayModes)
	{
		for (unsigned i = 0; i < displayModes.size(); ++i)
		{
			LOGI("Width: %d Height: %d Format: %d Refresh rate denominator: %d nominator: %d",
				displayModes[i].Width, displayModes[i].Height, displayModes[i].Format,
				displayModes[i].RefreshRate.Denominator, displayModes[i].RefreshRate.Numerator);

			if (displayModes[i].Width == m_window->GetWidth() &&
				displayModes[i].Height == m_window->GetHeight())
			{
				m_currentDisplayMode = displayModes[i];
				return;
			}
		}
	}


} // namespace engine