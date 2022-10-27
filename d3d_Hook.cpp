#include "d3d_Hook.hpp"

#include <MinHook.h>

bool Hook::Init()
{
	const std::string Title = E("SKYPE TERMINAL CALLING");

	WNDCLASSEX Window;

	Window.cbSize = sizeof(WNDCLASSEX);
	Window.style = CS_HREDRAW | CS_VREDRAW;

	Window.lpfnWndProc = DefWindowProcA;
	Window.cbClsExtra = NULL;
	Window.cbWndExtra = NULL;
	Window.hInstance = SAFE_CALL(GetModuleHandleA)(nullptr);
	Window.hIcon = NULL;
	Window.hCursor = NULL;
	Window.hbrBackground = NULL;
	Window.lpszMenuName = NULL;
	Window.lpszClassName = Title.c_str();
	Window.hIconSm = NULL;

	SAFE_CALL(RegisterClassExA)(&Window);

	HWND WindowA = CreateWindowA(Window.lpszClassName, Title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, Window.hInstance, 0);

	HMODULE D3D11_DLL;
	if ((D3D11_DLL = SAFE_CALL(GetModuleHandleA)(E("d3d11.dll"))) == NULL)
	{
		SAFE_CALL(DestroyWindow)(WindowA);
		SAFE_CALL(UnregisterClassA)(Window.lpszClassName, Window.hInstance);

		return false;
	}

	void* D3D11CreateDeviceAndSwapChain;

	if ((D3D11CreateDeviceAndSwapChain = SAFE_CALL(GetProcAddress)(D3D11_DLL, E("D3D11CreateDeviceAndSwapChain"))) == NULL)
	{
		SAFE_CALL(DestroyWindow)(WindowA);
		SAFE_CALL(UnregisterClassA)(Window.lpszClassName, Window.hInstance);

		return false;
	}

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };
	DXGI_RATIONAL refreshRate;
	refreshRate.Numerator = 60;
	refreshRate.Denominator = 1;

	DXGI_MODE_DESC bufferDesc;
	bufferDesc.Width = 100;
	bufferDesc.Height = 100;
	bufferDesc.RefreshRate = refreshRate;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = WindowA;
	swapChainDesc.Windowed = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	if (((long(__stdcall*)(
		IDXGIAdapter*,
		D3D_DRIVER_TYPE,
		HMODULE,
		UINT,
		const D3D_FEATURE_LEVEL*,
		UINT,
		UINT,
		const DXGI_SWAP_CHAIN_DESC*,
		IDXGISwapChain**,
		ID3D11Device**,
		D3D_FEATURE_LEVEL*,
		ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
	{
		SAFE_CALL(DestroyWindow)(WindowA);
		SAFE_CALL(UnregisterClassA)(Window.lpszClassName, Window.hInstance);

		return false;
	}

	g_methodsTable = (uint64_t*)calloc(205, sizeof(uint64_t));

	memcpy(g_methodsTable, *(uint64_t**)swapChain, 18 * sizeof(uint64_t));
	memcpy(g_methodsTable + 18, *(uint64_t**)device, 43 * sizeof(uint64_t));
	memcpy(g_methodsTable + 18 + 43, *(uint64_t**)context, 144 * sizeof(uint64_t));

	MH_Initialize();

	swapChain->Release();
	swapChain = 0;

	device->Release();
	device = 0;

	context->Release();
	context = 0;

	SAFE_CALL(DestroyWindow)(WindowA);
	SAFE_CALL(UnregisterClassA)(Window.lpszClassName, Window.hInstance);

	return true;
}

bool Hook::Present(void** hk_originalFunction, void* hk_hookedPresent)
{
	if (!hk_originalFunction || !hk_hookedPresent)
		return false;

	void* target = (void*)g_methodsTable[8];

	if (!target)
		return false;

	if (MH_CreateHook(target, hk_hookedPresent, hk_originalFunction) != MH_OK || MH_EnableHook(target) != MH_OK)
	{
		return false;
	}

	return true;
}