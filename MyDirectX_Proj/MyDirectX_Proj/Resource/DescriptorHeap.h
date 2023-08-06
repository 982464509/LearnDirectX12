#pragma once

#include"Resource.h"
#include <mutex>

class DescriptorHeap final : public Resource
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mHeap;
	D3D12_DESCRIPTOR_HEAP_DESC Desc;
	D3D12_CPU_DESCRIPTOR_HANDLE hCPUHeapStart;
	D3D12_GPU_DESCRIPTOR_HANDLE hGPUHeapStart;

	UINT HandleIncrementSize;
	UINT64 numDescriptors;
	std::vector<UINT> allocatePool;
	std::mutex heapMtx;

public:
	UINT64 Length() const
	{
		return numDescriptors; 
	}

	ID3D12DescriptorHeap* GetHeap() const 
	{ 
		return mHeap.Get();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE hGPU(UINT64 index) const
	{
		if (index >= Desc.NumDescriptors) index = Desc.NumDescriptors - 1;
		D3D12_GPU_DESCRIPTOR_HANDLE h = { hGPUHeapStart.ptr + index * HandleIncrementSize };
		return h;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE hCPU(UINT64 index) const
	{
		if (index >= Desc.NumDescriptors) index = Desc.NumDescriptors - 1;
		D3D12_CPU_DESCRIPTOR_HANDLE h = { hCPUHeapStart.ptr + index * HandleIncrementSize };
		return h;
	}

	DescriptorHeap(Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type, UINT64 numDescriptors, bool bShaderVisible);
	~DescriptorHeap();

	UINT AllocateIndex();

	void ReturnIndex(UINT v);
	void Reset();
	void CreateUAV(ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc, UINT64 index);
	void CreateSRV(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc, UINT64 index);
};
