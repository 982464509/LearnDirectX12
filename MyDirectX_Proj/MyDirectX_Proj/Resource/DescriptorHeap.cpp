//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include"DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(Device* device, D3D12_DESCRIPTOR_HEAP_TYPE Type, UINT64 numDescriptors, bool bShaderVisible)				
	: Resource(device),
	allocatePool(numDescriptors),
	numDescriptors(numDescriptors)
{
	Desc.Type = Type;
	Desc.NumDescriptors = numDescriptors;
	Desc.Flags = (bShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	Desc.NodeMask = 0;
	{
		for (size_t i = 0; i < numDescriptors; ++i)
		{
			allocatePool[i] = i;
		}
	}
	device->DxDevice()->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&mHeap));
				
	hCPUHeapStart = mHeap->GetCPUDescriptorHandleForHeapStart();
	hGPUHeapStart = mHeap->GetGPUDescriptorHandleForHeapStart();
	HandleIncrementSize = device->DxDevice()->GetDescriptorHandleIncrementSize(Desc.Type);
}

DescriptorHeap::~DescriptorHeap() {}

UINT DescriptorHeap::AllocateIndex()
{
	std::lock_guard lck(heapMtx);
#ifdef _DEBUG
	if (allocatePool.empty())
	{
		throw "bindless allocator out or range!\n";
	}
#endif
	auto last = allocatePool.end() - 1;
	UINT v = *last;
	allocatePool.erase(last);
	return v;
}

void DescriptorHeap::ReturnIndex(UINT v)
{
	std::lock_guard lck(heapMtx);
	allocatePool.emplace_back(v);
}

void DescriptorHeap::Reset()
{
	allocatePool.resize(numDescriptors);
	for (size_t i = 0; i < numDescriptors; ++i)
	{
		allocatePool[i] = i;
	}
}

void DescriptorHeap::CreateUAV(ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc, UINT64 index)
{
	device->DxDevice()->CreateUnorderedAccessView(resource, nullptr, &pDesc, hCPU(index));
}
void DescriptorHeap::CreateSRV(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc, UINT64 index)
{
	device->DxDevice()->CreateShaderResourceView(resource, &pDesc, hCPU(index));
}