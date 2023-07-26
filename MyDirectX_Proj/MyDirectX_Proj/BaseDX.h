#pragma once

#include <windows.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include "WindowsWapper.h"
#include "DXHelper.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


class BaseDX
{
public:
	BaseDX(uint32_t width, uint32_t height, std::wstring name);
	~BaseDX();

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }
	const wchar_t* GetTitle() const { return m_title.c_str(); }

protected:
	static const uint8_t FrameCount = 2;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CmdListAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

	uint32_t mRtvDescriptorSize = 0;
	uint32_t mDsvDescriptorSize = 0;
	uint32_t mCbvSrvUavDescriptorSize = 0;

	bool      m4xMsaaState = false;    // 4X MSAA enabled
	uint32_t      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	uint32_t m_width;
	uint32_t m_height;
	float m_aspectRatio;
	
	void SetCustomWindowText(LPCWSTR text);

private:
	std::wstring m_title;
};