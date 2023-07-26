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
	BaseDX(const BaseDX& rhs) = delete;
	BaseDX& operator=(const BaseDX& rhs) = delete;
	~BaseDX()=default;

	virtual void OnInit();
	virtual void OnResize();
	virtual void OnLoadAssets() = 0;
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

	virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }
	const wchar_t* GetTitle() const { return m_title.c_str(); }

protected:
	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

	//围栏
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	//命令队列
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	//命令分配器
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	//命令列表
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;

	//渲染目标视图 （描述符堆）
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	//深度/模板视图（描述符堆）
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	//交换链中的缓冲区
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	//用来记录当前后台缓冲区的索引
	int mCurrBackBuffer = 0;

	//视口
	D3D12_VIEWPORT mScreenViewport;
	//裁剪矩形
	D3D12_RECT mScissorRect;

	//rtv描述符大小，使用这个进行偏移计算进行迭代
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	uint32_t m_width;
	uint32_t m_height;
	float m_aspectRatio;

	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA
	
	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;

	void SetCustomWindowText(LPCWSTR text);	
	void FlushCommandQueue();

private:
	std::wstring m_title;
};