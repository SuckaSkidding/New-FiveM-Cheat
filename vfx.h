#pragma once
#include "ref_container.h"

#include <string>

namespace fx
{
	class Stream : public fwRefCountable
	{
	public:
		size_t Read(void* buffer, size_t length);

		uint64_t GetLength();

	public:
		static fwRefContainer<Stream> open_read(const std::string& path);
	};


} // fx