#pragma once

#include "Resource.h"


class Buffer : public Resource
{
protected:
	UINT64 bufferByteSize;

public:
	Buffer(Device* device);
	Buffer(Buffer&&) = default;
	virtual ~Buffer();

	
	virtual D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const = 0;
	virtual UINT64 GetByteSize() const;
};
