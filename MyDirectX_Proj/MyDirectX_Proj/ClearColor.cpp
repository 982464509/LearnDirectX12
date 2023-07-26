//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "ClearColor.h"


ClearColor::ClearColor(uint32_t width, uint32_t height, std::wstring name):BaseDX(width, height, name) {}


void ClearColor::OnInit()
{
	BaseDX::OnInit();
}

void ClearColor::OnLoadAssets()
{
}

void ClearColor::OnResize() 
{
    BaseDX::OnResize();
}

void ClearColor::OnUpdate() {}
void ClearColor::OnDestroy() {}

void ClearColor::OnRender()
{    
    mDirectCmdListAlloc->Reset();    
    mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr);
    auto p2t = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);        
    mCommandList->ResourceBarrier(1, &p2t);
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    //在每帧为了刷新场景而开始绘制之前，我们总是要清除后台缓冲区渲染目标和深度/模板缓冲区。
    // 清除后台缓冲区和深度缓冲区
    auto currentBackBufferView = CurrentBackBufferView();
    auto depthStencilView = DepthStencilView();
    mCommandList->ClearRenderTargetView(currentBackBufferView, Colors::Khaki, 0, nullptr); //nullptr表示清除整个渲染目标                
    mCommandList->ClearDepthStencilView(depthStencilView,
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,   //即将清除的是深度缓冲区还是模板缓冲区。用按位或运算符连接两者表示同时清除这两种缓冲区。
        1.0f,   //此值来清除深度缓冲区
        0,      //以此值来清除模板缓冲区。
        0,
        nullptr);

    // 指定设置将要渲染的缓冲区。（渲染目标和深度/模板缓冲区）
    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

    // 再次对资源状态进行转换，将资源从渲染目标状态转换回呈现状态
    auto t2p = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);        
    mCommandList->ResourceBarrier(1, &t2p);
    mCommandList->Close();  
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);   
    mSwapChain->Present(0, 0);
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
    FlushCommandQueue();
}