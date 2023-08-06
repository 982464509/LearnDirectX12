//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "DefaultBuffer.h"

DefaultBuffer::DefaultBuffer(Device* device, UINT64 byteSize, 
	D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_COMMON)
	: Buffer(device),
	initState(initState)	
{
	bufferByteSize = byteSize;
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		initState,
		nullptr,
		IID_PPV_ARGS(&resource)));
}


DefaultBuffer::~DefaultBuffer() {}

//void DefaultBuffer::DelayDispose(FrameResource* frameRes) const
//{
//	frameRes->AddDelayDisposeResource(resource);
//}