#pragma once

#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3d12shader.h>
#include <dxgi1_5.h>
#include <wrl/client.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

#define RELEASE(obj)  \
	if (obj != nullptr) { \
		obj->Release();   \
	}
