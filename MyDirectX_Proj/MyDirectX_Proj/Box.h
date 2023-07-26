#pragma once

#include "BaseDX.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;


struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class Box :public BaseDX
{
public:
	Box(uint32_t width, uint32_t height, std::wstring name);

	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

private:
	
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;

	//ComPtr<IDXGISwapChain3> m_swapChain;
	//ComPtr<ID3D12Device> m_device;
	//ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	//ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	//ComPtr<ID3D12CommandQueue> m_commandQueue;
	//ComPtr<ID3D12GraphicsCommandList> m_commandList;
	//ComPtr<ID3D12RootSignature> m_rootSignature;
	//ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	//ComPtr<ID3D12PipelineState> m_pipelineState;
	
	uint32_t m_rtvDescriptorSize;

	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	uint32_t m_frameIndex;
	HANDLE m_fenceEvent;
	//ComPtr<ID3D12Fence> m_fence;
	uint64_t m_fenceValue;

	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();
};