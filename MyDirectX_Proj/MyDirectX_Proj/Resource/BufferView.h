#pragma once

#include "Buffer.h"


class BufferView
{
public:
	BufferView() {}
	BufferView(Buffer const* buffer);
	BufferView(Buffer const* buffer, UINT64 offset, UINT64 byteSize);

	//一个指针，一个偏移，一个大小：
	Buffer const* buffer = nullptr;		
	UINT64 offset = 0;
	UINT64 byteSize = 0;

	/*bool operator==(BufferView const& a) const
	{
		return memcmp(this, &a, sizeof(BufferView)) == 0;
	}
	bool operator!=(BufferView const& a) const
	{
		return !operator==(a);
	}*/
};