#pragma once

#include <Windows.h>
#include <iostream>
#include <d3d11.h>

#include <vector>
#include <string>

#include <filesystem>
#include <fstream>

#include <mutex>
#include <map>
#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "EncryptString.hpp"
#include "EncryptFuncs.hpp"
#include "CustomWinApi.hpp"
#include "Memory.hpp"

#define M_PI 3.14159265358979323846

#pragma warning(disable: 4005)

#define DEBUG 0
#define SAFE_RELEASE(p) if (p) { p->Release(); p = nullptr; } 

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

template <typename T> T Read(uint64_t addr)
{
	T Novalue = {};
	if (!IsBadReadPtr((const void*)addr, sizeof(T)))
	{
		return *(T*)(addr);
	}
	else
	{
		return Novalue;
	}

};