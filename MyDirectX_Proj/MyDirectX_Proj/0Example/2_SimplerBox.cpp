//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "../0Example/2_SimplerBox.h"

//SimplerBox::SimplerBox(uint32_t width, uint32_t height, std::wstring name) :
//    BaseDX(width, height, name),
//    mLastMousePos({ 0, 0 })
//{    
//}
//
//
//void SimplerBox::OnInit()
//{
//    BaseDX::OnInit();
//
//    OnLoadAssets();
//    OnResize();
//}
//
//void SimplerBox::OnLoadAssets()
//{
//    // ���������б�Ϊִ�г�ʼ����������׼������
//    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
//    auto md3dDevice = mDevice->DxDevice();
//    //����������������
//    {
//        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
//        cbvHeapDesc.NumDescriptors = 1;
//        cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//        cbvHeapDesc.NodeMask = 0;
//        ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
//    }
//
//    mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice, 1, true);
//    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();    
//    int boxCBufIndex = 0;   // ƫ�Ƶ������������л��Ƶ�i����������ĳ������ݣ�����ȡi = 0    
//    cbAddress += boxCBufIndex * objCBByteSize;
//    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
//    cbvDesc.BufferLocation = cbAddress;
//    cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));   
//    md3dDevice->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
//
//    CD3DX12_ROOT_PARAMETER slotRootParameter[1];
//    CD3DX12_DESCRIPTOR_RANGE cbvTable;
//    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);// �������������������˻�ַ��ɫ���Ĵ���        
//    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable); 
//    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
//        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
//
//    Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
//    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
//    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());        
//    md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
//    
//
//    mvsByteCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
//    mpsByteCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");    
//    mInputLayout =
//    {
//        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
//    };
//
//    BuildBoxGeometry();
//
//    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
//    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
//    psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
//    psoDesc.pRootSignature = mRootSignature.Get();
//    psoDesc.VS =
//    {
//        reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
//        mvsByteCode->GetBufferSize()
//    };
//    psoDesc.PS =
//    {
//        reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
//        mpsByteCode->GetBufferSize()
//    };
//    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   //��դ���Ĺ�դ��״̬��
//    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
//    psoDesc.SampleMask = UINT_MAX;  //���ز������ɲɼ�32�������������еĲ����㶼���в�����
//    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; //ͼԪ����������
//    psoDesc.NumRenderTargets = 1;
//    psoDesc.RTVFormats[0] = mBackBufferFormat;
//    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
//    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
//    psoDesc.DSVFormat = mDepthStencilFormat;    
//    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
//    
//
//    mCommandList->Close();
//    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
//    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
//    FlushCommandQueue();
//}
//
//
//void SimplerBox::BuildBoxGeometry()
//{
//    std::array<Vertex, 8> vertices =
//    {
//        Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
//        Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
//        Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
//        Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
//        Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
//        Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
//        Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
//        Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
//    };
//    std::array<std::uint16_t, 36> indices =
//    {
//        0, 1, 2,// front face
//        0, 2, 3,
//        4, 6, 5,// back face
//        4, 7, 6,
//        4, 5, 1,// left face
//        4, 1, 0,
//        3, 2, 6,// right face
//        3, 6, 7,
//        1, 5, 6,// top face
//        1, 6, 2,
//        4, 0, 3,// bottom face
//        4, 3, 7
//    };
//
//    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
//    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);
//
//    mBoxGeo = std::make_unique<MeshGeometry>();
//    mBoxGeo->Name = "boxGeo";
//
//    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
//    CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
//    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
//    CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);
//
//    auto md3dDevice = mDevice->DxDevice();
//
//    mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
//        mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);
//
//    mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
//        mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);
//
//    mBoxGeo->VertexByteStride = sizeof(Vertex);
//    mBoxGeo->VertexBufferByteSize = vbByteSize;
//    mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
//    mBoxGeo->IndexBufferByteSize = ibByteSize;
//
//    SubmeshGeometry submesh;
//    submesh.IndexCount = (UINT)indices.size();
//    submesh.StartIndexLocation = 0;
//    submesh.BaseVertexLocation = 0;
//
//    mBoxGeo->DrawArgs["box"] = submesh;
//}
//
//void SimplerBox::OnResize()
//{
//    BaseDX::OnResize();
//
//    auto aspectRatio = static_cast<float>(m_width) / m_height;
//    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, aspectRatio, 1.0f, 1000.0f);
//    XMStoreFloat4x4(&mProj, P);
//}
//
//void SimplerBox::OnUpdate() 
//{
//    float x = mRadius * sinf(mPhi) * cosf(mTheta);
//    float z = mRadius * sinf(mPhi) * sinf(mTheta);
//    float y = mRadius * cosf(mPhi);
//
//    // �����۲����
//    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
//    XMVECTOR target = XMVectorZero();
//    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
//    XMStoreFloat4x4(&mView, view);
//
//    XMMATRIX world = XMLoadFloat4x4(&mWorld);
//    XMMATRIX proj = XMLoadFloat4x4(&mProj);
//    XMMATRIX worldViewProj = world * view * proj;
//
//    // �����µ�worldViewProj ���������³���������
//    ObjectConstants objConstants;
//    XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
//    objConstants.ColorW.x = 0.1;
//
//    mObjectCB->CopyData(0, objConstants);
//}
//
//void SimplerBox::OnDestroy() 
//{
//    BaseDX::OnDestroy();
//}
//
//void SimplerBox::OnRender()
//{
//    mDirectCmdListAlloc->Reset();
//    mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get());
//    auto p2t = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
//        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//    mCommandList->ResourceBarrier(1, &p2t);
//    mCommandList->RSSetViewports(1, &mScreenViewport);
//    mCommandList->RSSetScissorRects(1, &mScissorRect);   
//    auto currentBackBufferView = CurrentBackBufferView();
//    auto depthStencilView = DepthStencilView();
//    mCommandList->ClearRenderTargetView(currentBackBufferView, Colors::Khaki, 0, nullptr);            
//    mCommandList->ClearDepthStencilView(depthStencilView,
//        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 
//        1.0f,   //��ֵ�������Ȼ�����
//        0,      //�Դ�ֵ�����ģ�建������
//        0,
//        nullptr);
//    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);
//
//
//
//    {
//        auto vv = mBoxGeo->VertexBufferView();
//        auto iv = mBoxGeo->IndexBufferView();
//        ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
//        mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
//        mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
//        mCommandList->IASetVertexBuffers(0, 1, &vv);
//        mCommandList->IASetIndexBuffer(&iv);
//        mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//        mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
//        mCommandList->DrawIndexedInstanced(mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);
//    }
//
//
//
//
//    // �ٴζ���Դ״̬����ת��������Դ����ȾĿ��״̬ת���س���״̬
//    auto t2p = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
//        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//    mCommandList->ResourceBarrier(1, &t2p);
//    mCommandList->Close();
//    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
//    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
//    mSwapChain->Present(0, 0);
//    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
//    FlushCommandQueue();
//}


//void SimplerBox::OnMouseDown(WPARAM btnState, int x, int y)
//{
//    mLastMousePos.x = x;
//    mLastMousePos.y = y;
//
//    SetCapture(WinApplication::GetHwnd());
//}
//
//void SimplerBox::OnMouseUp(WPARAM btnState, int x, int y)
//{
//    ReleaseCapture();
//}
//
//void SimplerBox::OnMouseMove(WPARAM btnState, int x, int y)
//{
//    if ((btnState & MK_LBUTTON) != 0)
//    {
//        // ���������ƶ����������ת�Ƕȣ���ÿ�����ذ��˽Ƕȵ�1/4������ת
//        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
//        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));
//
//        // ���������������������������������ת�ĽǶ�
//        mTheta += dx;
//        mPhi += dy;
//
//        // ���ƽǶ�mPhi�ķ�Χ
//        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
//    }
//    else if ((btnState & MK_RBUTTON) != 0)
//    {
//        // ʹ�����е�ÿ�����ذ�����ƶ������0.005����������
//        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
//        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);
//
//        // ���������������������Ŀ��ӷ�Χ�뾶
//        mRadius += dx - dy;
//        // ���ƿ��Ӱ뾶�ķ�Χ
//        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
//    }
//
//    mLastMousePos.x = x;
//    mLastMousePos.y = y;
//}