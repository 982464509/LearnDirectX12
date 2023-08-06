#pragma once

#include <windows.h>
#include <dxgi1_6.h>
#include "../d3dx12.h"
#include <dxgi1_4.h>
#include <d3d12.h>
#include <functional>


class FrameResource
{
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> delayDisposeResources;
	std::vector<std::function<void()>> afterSyncEvents;
	UINT64 lastFenceIndex = 0;




public:
	FrameResource();
	~FrameResource();


};
