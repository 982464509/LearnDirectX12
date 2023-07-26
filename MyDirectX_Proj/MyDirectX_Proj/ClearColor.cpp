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

    //��ÿ֡Ϊ��ˢ�³�������ʼ����֮ǰ����������Ҫ�����̨��������ȾĿ������/ģ�建������
    // �����̨����������Ȼ�����
    auto currentBackBufferView = CurrentBackBufferView();
    auto depthStencilView = DepthStencilView();
    mCommandList->ClearRenderTargetView(currentBackBufferView, Colors::Khaki, 0, nullptr); //nullptr��ʾ���������ȾĿ��                
    mCommandList->ClearDepthStencilView(depthStencilView,
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,   //�������������Ȼ���������ģ�建�������ð�λ��������������߱�ʾͬʱ��������ֻ�������
        1.0f,   //��ֵ�������Ȼ�����
        0,      //�Դ�ֵ�����ģ�建������
        0,
        nullptr);

    // ָ�����ý�Ҫ��Ⱦ�Ļ�����������ȾĿ������/ģ�建������
    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

    // �ٴζ���Դ״̬����ת��������Դ����ȾĿ��״̬ת���س���״̬
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