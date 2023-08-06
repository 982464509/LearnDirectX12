//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include"UploadBuffer.h"

UploadBuffer::UploadBuffer(Device* device, UINT64 byteSize)		
	: Buffer(device)
{
	bufferByteSize = byteSize;
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource)));
}

UploadBuffer::~UploadBuffer(){}

//template<typename T>
//void UploadBuffer::CopyData(UINT64 offset, const T& data) const
//{
//	void* mappedPtr;
//	D3D12_RANGE range;
//	range.Begin = offset;
//	range.End = min(byteSize, offset + data.size());
//	ThrowIfFailed(resource->Map(0, &range, reinterpret_cast<void**>(&mappedPtr)));
//	memcpy(reinterpret_cast<vbyte*>(mappedPtr) + offset, data.data(), range.End - range.Begin);
//	resource->Unmap(0, &range);
//}

//void UploadBuffer::DelayDispose(FrameResource* frameRes) const
//{
//	frameRes->AddDelayDisposeResource(resource);
//}