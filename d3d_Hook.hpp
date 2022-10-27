#pragma once

#include "includes.hpp"

namespace Hook
{
	bool Present(void** hk_originalFunction, void* hk_hookedPresent);
	bool Init();
}

static uint64_t* g_methodsTable = 0;

namespace DirectX
{
	static bool OverlayHooked = false;
	static Present OriginalPresent = 0;
	static HWND Window = 0;
	static WNDPROC WindowEx = 0;
	static ID3D11Device* pDevice = 0;
	static ID3D11DeviceContext* pContext = 0;
	static ID3D11RenderTargetView* renderTargetView = 0;
}