#pragma once

#include "../Base/BaseDX.h"


class Device
{	
public:
	Device();
	~Device();
	
	ID3D12Device* DxDevice() const { return md3dDevice.Get(); }
	IDXGIFactory4* DxgiFactory() const { return mdxgiFactory.Get(); }
	//IDXGIAdapter1* Adapter() const { return pWarpAdapter.Get(); }

private:	
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;

	//Microsoft::WRL::ComPtr<IDXGIAdapter1> pWarpAdapter;
};
