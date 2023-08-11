//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "MaterialFrameResource.h"

MaterialFrameResource::MaterialFrameResource(
    ID3D12Device* device, 
    UINT passCount, 
    UINT objectCount, 
    UINT materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CmdListAlloc.GetAddressOf())));

    //  FrameCB = std::make_unique<UploadBuffer<FrameConstants>>(device, 1, true);
    PassCB = std::make_unique<UploadBuffer<LightPassConstants>>(device, passCount, true);
    MaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, materialCount, true);
    ObjectCB = std::make_unique<UploadBuffer<WTObjectConstants>>(device, objectCount, true);
}

MaterialFrameResource::~MaterialFrameResource() {}