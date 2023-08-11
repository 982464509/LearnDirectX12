//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include"04_DynamicBox.h"

DynamicBox::DynamicBox(uint32_t width, uint32_t height, std::wstring name) :
    BaseDX(width, height, name),
    mLastMousePos({ 0, 0 })
{
}


void DynamicBox::OnInit()
{
    BaseDX::OnInit();
    OnResize();    

    mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr);

    OnLoadAssets();    

    mCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    FlushCommandQueue();
}

void DynamicBox::OnLoadAssets()
{        
    CD3DX12_ROOT_PARAMETER slotRootParameter[2];
    slotRootParameter[0].InitAsConstantBufferView(0);
    slotRootParameter[1].InitAsConstantBufferView(1);

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

    mDevice->DxDevice()->CreateRootSignature(0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature));
    
    //创建常量描述符堆
    {
       /* D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
        cbvHeapDesc.NumDescriptors = 1;
        cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        cbvHeapDesc.NodeMask = 0;
        ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));*/
    }
   
    auto path = L"E:\\01_DirectX\\DirectX12_Proj\\MyDirectX_Proj\\MyDirectX_Proj\\Shaders\\3_color.hlsl";
    mShaders["standardVS"] = DXHelper::CompileShader(path, nullptr, "VS", "vs_5_0");
    mShaders["opaquePS"] = DXHelper::CompileShader(path, nullptr, "PS", "ps_5_0");
    mInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    BuildBoxGeometry();

    {
        auto boxRitem1 = std::make_unique<BoxRenderItem>();
        XMStoreFloat4x4(&boxRitem1->World, XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(2.0, 0.0f, 0.0f));
        boxRitem1->ObjCBIndex = 0;
        boxRitem1->Geo = mGeometries["shapeGeo"].get();
        boxRitem1->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        auto box1 = boxRitem1->Geo->DrawArgs["box1"];
        boxRitem1->IndexCount = box1.IndexCount;
        boxRitem1->StartIndexLocation = box1.StartIndexLocation;
        boxRitem1->BaseVertexLocation = box1.BaseVertexLocation;
        mAllRitems.push_back(std::move(boxRitem1));

        auto boxRitem2 = std::make_unique<BoxRenderItem>();
        XMStoreFloat4x4(&boxRitem2->World, XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-2.0f, 0.0f, 0.0f));
        boxRitem2->ObjCBIndex = 1;
        boxRitem2->Geo = mGeometries["shapeGeo"].get();
        boxRitem2->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        auto box2 = boxRitem2->Geo->DrawArgs["box2"];
        boxRitem2->IndexCount = box2.IndexCount;
        boxRitem2->StartIndexLocation = box2.StartIndexLocation;
        boxRitem2->BaseVertexLocation = box2.BaseVertexLocation;
        mAllRitems.push_back(std::move(boxRitem2));
        

        for (auto& e : mAllRitems)
            mOpaqueRitems.push_back(e.get());
    }

    {
        for (int i = 0; i < gNumFrameResources; ++i)
        {
            mFrameResources.push_back(std::make_unique<FrameResource>(mDevice->DxDevice(), 1, (UINT)mAllRitems.size()));
        }
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
    psoDesc.pRootSignature = mRootSignature.Get();
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
        mShaders["standardVS"]->GetBufferSize()
    };
    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
        mShaders["opaquePS"]->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   //光栅器的光栅化状态。
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;  //多重采样最多可采集32个样本。对所有的采样点都进行采样。
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; //图元的拓扑类型
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = mBackBufferFormat;
    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = mDepthStencilFormat;
    ThrowIfFailed(mDevice->DxDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));

    D3D12_GRAPHICS_PIPELINE_STATE_DESC opaqueWireframePsoDesc = psoDesc;
    opaqueWireframePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
    ThrowIfFailed(mDevice->DxDevice()->CreateGraphicsPipelineState(&opaqueWireframePsoDesc, IID_PPV_ARGS(&mPSOs["opaque_wireframe"])));    
}


void DynamicBox::BuildBoxGeometry()
{
    GeometryGenerator geoGen;
    GeometryGenerator::MeshData box1 = geoGen.CreateBox(1.5f, 1.5f, 1.5f, 3);
    GeometryGenerator::MeshData box2 = geoGen.CreateBox(1.5f, 1.5f, 1.5f, 3);

    UINT box1VertexOffset = 0;
    UINT box2VertexOffset = (UINT)box1.Vertices.size();

    UINT box1IndexOffset = 0;
    UINT box2IndexOffset = (UINT)box1.Indices32.size();

    SubmeshGeometry box1Submesh;
    box1Submesh.IndexCount = (UINT)box1.Indices32.size();
    box1Submesh.StartIndexLocation = box1IndexOffset;
    box1Submesh.BaseVertexLocation = box1VertexOffset;

    SubmeshGeometry box2dSubmesh;
    box2dSubmesh.IndexCount = (UINT)box2.Indices32.size();
    box2dSubmesh.StartIndexLocation = box2IndexOffset;
    box2dSubmesh.BaseVertexLocation = box2VertexOffset;

    auto totalVertexCount =
        box1.Vertices.size() +
        box2.Vertices.size();

    std::vector<Vertex> vertices(totalVertexCount);

    UINT k = 0;
    for (size_t i = 0; i < box1.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos = box1.Vertices[i].Position;
        vertices[k].Color = XMFLOAT4(DirectX::Colors::DarkGreen);
    }

    for (size_t i = 0; i < box2.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos = box2.Vertices[i].Position;
        vertices[k].Color = XMFLOAT4(DirectX::Colors::RosyBrown);
    }
    std::vector<std::uint16_t> indices;
    indices.insert(indices.end(), std::begin(box1.GetIndices16()), std::end(box1.GetIndices16()));
    indices.insert(indices.end(), std::begin(box2.GetIndices16()), std::end(box2.GetIndices16()));

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    auto geo = std::make_unique<MeshGeometry>();
    geo->Name = "shapeGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
    CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
    CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    geo->VertexBufferGPU = DXHelper::CreateDefaultBuffer(mDevice->DxDevice(),
        mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

    geo->IndexBufferGPU = DXHelper::CreateDefaultBuffer(mDevice->DxDevice(),
        mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

    geo->VertexByteStride = sizeof(Vertex);
    geo->VertexBufferByteSize = vbByteSize;

    geo->IndexFormat = DXGI_FORMAT_R16_UINT;
    geo->IndexBufferByteSize = ibByteSize;

    geo->DrawArgs["box1"] = box1Submesh;
    geo->DrawArgs["box2"] = box2dSubmesh;
    
    mGeometries[geo->Name] = std::move(geo);
}

void DynamicBox::OnResize()
{
    BaseDX::OnResize();

    auto aspectRatio = static_cast<float>(m_width) / m_height;
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, aspectRatio, 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void DynamicBox::OnUpdate()
{
    if (GetAsyncKeyState('1') & 0x8000)
        mIsWireframe = true;
    else
        mIsWireframe = false;

    mTheta -= 0.0005;   
    
    mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
    mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
    mEyePos.y = mRadius * cosf(mPhi);

    // Build the view matrix.
    XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
    mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

    UINT64 _fence = mCurrFrameResource->Fence;
    if (_fence != 0)
    {
        if (_fence > mFence->GetCompletedValue())
        {

            HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
            ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);
        }
    }

    // UpdateObjectCBs
    {
        auto currObjectCB = mCurrFrameResource->ObjectCB.get();
        for (auto& renderItem : mAllRitems)
        {
            if (renderItem->NumFramesDirty > 0)
            {
                XMMATRIX world = XMLoadFloat4x4(&renderItem->World);
                ObjectConstants objConstants;
                XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
                currObjectCB->CopyData(renderItem->ObjCBIndex, objConstants);
                // 还需要对下一个FrameResource进行更新
                renderItem->NumFramesDirty--;
            }
        }
    }

    // UpdateMainPassCB
    {
        XMMATRIX view = XMLoadFloat4x4(&mView);
        XMMATRIX proj = XMLoadFloat4x4(&mProj);

        XMMATRIX viewProj = XMMatrixMultiply(view, proj);

        auto vv = XMMatrixDeterminant(view);
        auto pj = XMMatrixDeterminant(proj);
        auto vj = XMMatrixDeterminant(viewProj);
        XMMATRIX invView = XMMatrixInverse(&vv, view);
        XMMATRIX invProj = XMMatrixInverse(&pj, proj);
        XMMATRIX invViewProj = XMMatrixInverse(&vj, viewProj);

        PassConstants mMainPassCB;
        XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
        XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
        XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
        XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
        XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
        XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
        mMainPassCB.EyePosW = mEyePos;
        mMainPassCB.RenderTargetSize = XMFLOAT2((float)m_width, (float)m_height);
        mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / m_width, 1.0f / m_height);
        mMainPassCB.NearZ = 1.0f;
        mMainPassCB.FarZ = 1000.0f;

        auto currPassCB = mCurrFrameResource->PassCB.get();
        currPassCB->CopyData(0, mMainPassCB);
    }
}

void DynamicBox::OnDestroy()
{
    BaseDX::OnDestroy();
}

void DynamicBox::OnRender()
{
    auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;
    cmdListAlloc->Reset();

   if (mIsWireframe)
    {
        ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque_wireframe"].Get()));
    }
    else
    {
        ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));
    }    
   
   mCommandList->RSSetViewports(1, &mScreenViewport);
   mCommandList->RSSetScissorRects(1, &mScissorRect);

    auto p2t = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &p2t);

    auto currentBackBufferView = CurrentBackBufferView();
    auto depthStencilView = DepthStencilView();
    mCommandList->ClearRenderTargetView(currentBackBufferView, Colors::Khaki, 0, nullptr);
    mCommandList->ClearDepthStencilView(depthStencilView,
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
        1.0f,   //此值来清除深度缓冲区
        0,      //以此值来清除模板缓冲区。
        0,
        nullptr);
    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);



    {
        /*auto vv = mBoxGeo->VertexBufferView();
        auto iv = mBoxGeo->IndexBufferView();
        ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
        mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
        mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
        mCommandList->IASetVertexBuffers(0, 1, &vv);
        mCommandList->IASetIndexBuffer(&iv);
        mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
        mCommandList->DrawIndexedInstanced(mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);*/

        mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
        auto passCB = mCurrFrameResource->PassCB->Resource();
        mCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());

        {
            UINT objCBByteSize = DXHelper::CalcConstantBufferByteSize(sizeof(ObjectConstants));
            auto objectCB = mCurrFrameResource->ObjectCB->Resource();
            for (size_t i = 0; i < mOpaqueRitems.size(); ++i)
            {
                auto ri = mOpaqueRitems[i];
                auto vbv = ri->Geo->VertexBufferView();
                auto ibv = ri->Geo->IndexBufferView();
                mCommandList->IASetVertexBuffers(0, 1, &vbv);
                mCommandList->IASetIndexBuffer(&ibv);
                mCommandList->IASetPrimitiveTopology(ri->PrimitiveType);

                D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress();
                objCBAddress += ri->ObjCBIndex * objCBByteSize;

                //设置根描述符,将根描述符与资源绑定//寄存器槽号//子资源地址
                mCommandList->SetGraphicsRootConstantBufferView(0, objCBAddress);

                //绘制顶点（通过索引缓冲区绘制）
                mCommandList->DrawIndexedInstanced(
                    ri->IndexCount,		//每个实例要绘制的索引数
                    1, //实例化个数
                    ri->StartIndexLocation, //起始索引位置
                    ri->BaseVertexLocation, //子物体起始索引在全局索引中的位置
                    0);		//实例化的高级技术，暂时设置为0
            }
        }
    }




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


void DynamicBox::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(WinApplication::GetHwnd());
}

void DynamicBox::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void DynamicBox::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0)
    {
        // 根据鼠标的移动距离计算旋转角度，令每个像素按此角度的1/4进行旋转
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

        // 根据鼠标的输入来更新摄像机绕立方体旋转的角度
        mTheta -= dx;
        mPhi -= dy;        
        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);// 限制角度mPhi的范围
    }
    else if ((btnState & MK_RBUTTON) != 0)
    {
        // 使场景中的每个像素按鼠标移动距离的0.005倍进行缩放
        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

        // 根据鼠标的输入更新摄像机的可视范围半径
        mRadius += dx - dy;
        // 限制可视半径的范围
        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}