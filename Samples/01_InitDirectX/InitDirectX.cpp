//***************************************************************************************
// @author : zxz
// @file :
// @description : ��ʼ��DX12
//***************************************************************************************


#include "../../Common/d3dApp.h"
#include <DirectXColors.h>

using namespace DirectX;

class InitDirect3D :public D3DApp
{
public:
    InitDirect3D(HINSTANCE hInstance);
    ~InitDirect3D();

    virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(const GameTimer& gt)override;
    virtual void Draw(const GameTimer& gt)override;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    // Ϊ���Թ�����������ʱ�ڴ��顣
#if defined(DEBUG) | defined(_DEBUG)
      //�������޸�_crtDbgFlag��־��״̬���Կ��Ƶ��Զѹ������ķ�����Ϊ�������԰汾����
       //��_DEBUGû�ж���ʱ����_CrtSetDbgFlag�ĵ�����Ԥ��������б�ɾ����
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    InitDirect3D theApp(hInstance);
    theApp.Initialize();
    return theApp.Run();
}

InitDirect3D::InitDirect3D(HINSTANCE hInstance) : D3DApp(hInstance)
{
}

InitDirect3D::~InitDirect3D()
{
}

bool InitDirect3D::Initialize()
{
    return D3DApp::Initialize();
}

void InitDirect3D::OnResize()
{
    D3DApp::OnResize();
}

void InitDirect3D::Update(const GameTimer& gt)
{
}

void InitDirect3D::Draw(const GameTimer& gt)
{
    // �ظ�ʹ�ü�¼���������ڴ�
// ֻ�е���GPU�����������б�ִ�����ʱ�����ǲ��ܽ�������
    mDirectCmdListAlloc->Reset();
    // ��ͨ��ExecuteCommandList������ĳ�������б����������к����Ǳ�������ø������б���
    // �������������б����ڴ�
    mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr);

    // ����Դ��״̬����ת��������Դ�ӳ���״̬ת��Ϊ��ȾĿ��״̬
    auto resourcePresent2Target = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &resourcePresent2Target);

    // �����ӿںͲü����Ρ�������Ҫ���������б�����ö�����
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    //��ÿ֡Ϊ��ˢ�³�������ʼ����֮ǰ����������Ҫ�����̨��������ȾĿ������/ģ�建������
    // �����̨����������Ȼ�����
    auto currentBackBufferView = CurrentBackBufferView();
    auto depthStencilView = DepthStencilView();

    mCommandList->ClearRenderTargetView(currentBackBufferView, Colors::CornflowerBlue, 0, nullptr); //nullptr��ʾ���������ȾĿ��                
    mCommandList->ClearDepthStencilView(depthStencilView,
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,   //�������������Ȼ���������ģ�建�������ð�λ��������������߱�ʾͬʱ��������ֻ�������
        1.0f,   //��ֵ�������Ȼ�����
        0,      //�Դ�ֵ�����ģ�建������
        0,
        nullptr);

    // ָ�����ý�Ҫ��Ⱦ�Ļ�����������ȾĿ������/ģ�建������
    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

    // �ٴζ���Դ״̬����ת��������Դ����ȾĿ��״̬ת���س���״̬
    auto resourceTarget2Present = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &resourceTarget2Present);

    // ������ϣ�

    // �������ļ�¼
    mCommandList->Close();   
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };     // ����ִ�е������б�����������
    // ��ʽ�ύ��
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);   

    // ������̨��������ǰ̨������
    mSwapChain->Present(0, 0);   
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // �ȴ���֡������ִ����ϡ���ǰ��ʵ��û��ʲôЧ�ʣ�Ҳ���ڼ������ں��潫������֯��Ⱦ���ֵĴ��룬������ÿһ֡��Ҫ�ȴ�    
    FlushCommandQueue();
}