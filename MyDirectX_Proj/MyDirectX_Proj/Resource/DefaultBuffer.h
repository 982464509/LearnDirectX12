#pragma once

#include"Buffer.h"

class DefaultBuffer final:public Buffer
{
private:
	D3D12_RESOURCE_STATES initState;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

public:
	DefaultBuffer(Device* device, UINT64 byteSize, D3D12_RESOURCE_STATES initState);		
	DefaultBuffer(DefaultBuffer&&) = default;
	DefaultBuffer(DefaultBuffer const&) = delete;
	~DefaultBuffer();

	ID3D12Resource* GetResource() const override
	{
		return resource.Get();
	}

	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const override
	{
		return resource->GetGPUVirtualAddress();
	}

	D3D12_RESOURCE_STATES GetInitState() const override
	{
		return initState;
	}

	// void DelayDispose(FrameResource* frameRes) const override;
};
