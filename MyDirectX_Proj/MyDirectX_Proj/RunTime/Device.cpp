//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "Device.h"

Device::Device()
{
	CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory));
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&md3dDevice));
}

Device::~Device(){}