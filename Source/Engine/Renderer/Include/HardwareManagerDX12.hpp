#pragma once

#include "D3DIncludes.hpp"
#include "Types.hpp"
#include "IWindow.hpp"

#include <vector>


typedef std::vector<ComPtr<IDXGIAdapter>> AdapterVector_t;
typedef std::vector<ComPtr<IDXGIOutput>> OutputVector_t;
typedef std::vector<DXGI_MODE_DESC> DisplayModeList_t;

namespace engine {

	class HardwareManager {
	public:
		HardwareManager(ComPtr<IDXGIFactory4>& factory, IWindow* window)
			: m_factory(factory)
			, m_window(window) {}

		~HardwareManager() {}

		Bool Initialize();
		void GetAdapters(AdapterVector_t& outAdapters) const;
		void GetOutputs(ComPtr<IDXGIAdapter>& adapter, OutputVector_t& outAdapterOutputs) const;
		Bool GetDisplayModes(ComPtr<IDXGIOutput>& output, DisplayModeList_t& outDisplayModes) const;
		Bool ChooseDisplayMode(const DisplayModeList_t& displayModes);

		ComPtr<IDXGIAdapter>& GetCurrentAdapter() { return m_currentAdapter; }
		ComPtr<IDXGIOutput>& GetCurrentOutput() { return m_currentOutput; }
		DXGI_MODE_DESC GetCurrentDisplayMode() { return m_currentDisplayMode; }

	private:
		ComPtr<IDXGIFactory4> m_factory;
		IWindow* m_window;
		ComPtr<IDXGIAdapter> m_currentAdapter;
		ComPtr<IDXGIOutput> m_currentOutput;
		DXGI_MODE_DESC m_currentDisplayMode;
	};

} // namespace engine
