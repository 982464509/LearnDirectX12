#pragma once

#include"Buffer.h"

class ReadbackBuffer final : public Buffer
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

public:
	ReadbackBuffer(Device* device, UINT64 byteSize);
	ReadbackBuffer(ReadbackBuffer&&) = default;
	ReadbackBuffer(ReadbackBuffer const&) = delete;
	~ReadbackBuffer();

	template<typename T>
	void CopyData(UINT64 offset, const T& data) const;

	//void DelayDispose(FrameResource * frameRes) const override;

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
		return D3D12_RESOURCE_STATE_COPY_DEST;
	}	
};
