#include "vfx.h"

#include <Windows.h>
#include <string>
#pragma once
#include <cstdint>

namespace util
{
	inline void* get_unhooked_ptr(void* ptr)
	{
		if (*(uint8_t*)ptr == 0xF1)
			return (void*)((uint64_t)ptr + 0x1);

		return ptr;
	}
}
namespace fx
{
	fwRefContainer<Stream> Stream::open_read(const std::string& path)
	{
		static fwRefContainer<Stream>(*open_read)(const std::string & path) =
			(decltype(open_read))util::get_unhooked_ptr(GetProcAddress(GetModuleHandle("vfs-core.dll"), MAKEINTRESOURCE(102)));

		return open_read(path);
	}

	size_t Stream::Read(void* buffer, size_t length)
	{
		return (*(size_t(__fastcall**)(uint64_t, uint64_t, void*, __int64))(**((uint64_t**)this + 2) + 48i64))(*((uint64_t*)this + 2), *((uint64_t*)this + 3), buffer, length);
	}

	uint64_t Stream::GetLength()
	{
		return (*(uint64_t(__fastcall**)(uint64_t, uint64_t))(**((uint64_t**)this + 2) + 152i64))(*((uint64_t*)this + 2), *((uint64_t*)this + 3));
	}
} // fx