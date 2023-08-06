#pragma once

#include"Buffer.h"


class UploadBuffer final : public Buffer
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

public:
	UploadBuffer(Device* device, UINT64 byteSize);			
	UploadBuffer(UploadBuffer&&) = default;
	UploadBuffer(UploadBuffer const&) = delete;
	~UploadBuffer();

	template<typename T>
	void CopyData(UINT64 offset, const T& data) const;

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
		return D3D12_RESOURCE_STATE_GENERIC_READ;
	}

	//void DelayDispose(FrameResource* frameRes) const override;
};
