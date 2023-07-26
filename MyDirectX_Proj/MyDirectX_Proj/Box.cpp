//***************************************************************************************
// @author : zxz
// @file :
// @description :绘制Box
//***************************************************************************************

#include "Box.h"


Box::Box(uint32_t width, uint32_t height, std::wstring name):
	BaseDX(width, height, name),
	m_frameIndex(0),
	m_scissorRect(0, 0, m_width, m_height)
{	
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(m_width);
	m_viewport.Height = static_cast<float>(m_height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}

void Box::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

void Box::OnUpdate(){}

void Box::OnRender()
{
	// Record all the commands we need to render the scene into the command list.
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	ThrowIfFailed(m_swapChain->Present(1, 0));

	WaitForPreviousFrame();
}

void Box::OnDestroy()
{
	WaitForPreviousFrame();
	CloseHandle(m_fenceEvent);
}



void Box::LoadPipeline()
{
	CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory));

	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             //指定在创建设备时所用的显示适配器。若将此参数设定为空指针，则使用主显示适配器。
		D3D_FEATURE_LEVEL_11_0,			//最低支持功能
		IID_PPV_ARGS(&md3dDevice));

	md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	md3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels));
	m4xMsaaQuality = msQualityLevels.NumQualityLevels;

	// Create CommandObjects
	auto command_direct = D3D12_COMMAND_LIST_TYPE_DIRECT; //接受所有类型的命令

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = command_direct;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	md3dDevice->CreateCommandAllocator(command_direct, IID_PPV_ARGS(m_CmdListAllocator.GetAddressOf()));
	

	// Create SwapChain
	m_swapChain.Reset();	
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};	//描述创建后的交换链的特性
	swapChainDesc.Width = m_width;		// 缓冲区分辨率
	swapChainDesc.Height = m_height;
	// sd.BufferDesc.RefreshRate.Numerator = 60;
	// sd.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.Format = mBackBufferFormat;	// 缓冲区的显示格式
	// sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// 逐行扫描 vs. 隔行扫描
	// sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	 // 图像如何相对于屏幕进行拉伸
	swapChainDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	swapChainDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//用它作为渲染目标，所以采用这个
	swapChainDesc.BufferCount = FrameCount;

	//sd.OutputWindow = mhMainWnd;	//渲染窗口的句柄
	//sd.Windowed = true;	//若为true，程序将在窗口模式下运行；如果指定为false，则采用全屏模式。
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	//当程序切换为全屏模式时，它将选择最适于当前应用程序窗口尺寸的显示模式。
	//如果没有指定该标志，当程序切换为全屏模式时，将采用当前桌面的显示模式。
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//注意，交换链需要通过命令队列对其进行刷新
	//mdxgiFactory->CreateSwapChain(mCommandQueue.Get(), &sd, &m_swapChain);

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(mdxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		WinApplication::GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain));
	ThrowIfFailed(mdxgiFactory->MakeWindowAssociation(WinApplication::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
	ThrowIfFailed(swapChain.As(&m_swapChain));
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();


	// Create descriptor heaps.
	{
		//为交换链中SwapChainBufferCount个用于渲染数据的缓冲区资源创建对应的渲染目标视图（Render Target View，RTV）	
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = FrameCount;	 // 这个描述符堆用来存储SwapChainBufferCount个RTV
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_rtvHeap.GetAddressOf()));
	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());	
		for (auto n = 0; n < FrameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			md3dDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}
}

void Box::LoadAssets()
{
	// Create an empty root signature.
	{
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		ThrowIfFailed(md3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}

	// Create the pipeline state, which includes compiling and loading shaders.
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		uint32_t compileFlags = 0;
#endif

		vertexShader = DXHelper::CompileShader(L"shader\\shaders.hlsl", nullptr, "VSMain", "vs_5_0");
		pixelShader = DXHelper::CompileShader(L"shader\\shaders.hlsl", nullptr, "PSMain", "ps_5_0");


		/*ThrowIfFailed(DXHelper::CompileShader(GetAssetFullPath(L"shader/shaders.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shader/shaders.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));*/

		// Define the vertex input layout.
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0} 
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};	
		psoDesc.InputLayout = { inputElementDescs.data(), (UINT)inputElementDescs.size() };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
	}
	md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdListAllocator.Get(),
		m_pipelineState.Get(),
		IID_PPV_ARGS(m_commandList.GetAddressOf()));
	m_commandList->Close();	//先关闭


	// Create the vertex buffer.
	{
		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{{0.0f, 0.25f * m_aspectRatio, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{0.25f, -0.25f * m_aspectRatio, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
			{{-0.25f, -0.25f * m_aspectRatio, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} };

		const uint32_t vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not
		// recommended. Every time the GPU needs it, the upload heap will be marshalled
		// over. Please read up on Default Heap usage. An upload heap is used here for
		// code simplicity and because there are very few verts to actually transfer.

		auto s1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto s2 = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		ThrowIfFailed(md3dDevice->CreateCommittedResource(
			&s1,
			D3D12_HEAP_FLAG_NONE,
			&s2,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)));

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);// We do not intend to read from this resource on the CPU.
		ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}

	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
		ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		// Wait for the command list to execute; we are reusing the same command
		// list in our main loop but for now, we just want to wait for setup to
		// complete before continuing.
		WaitForPreviousFrame();
	}
}
void Box::PopulateCommandList()
{
	ThrowIfFailed(m_CmdListAllocator->Reset());

	// However, when ExecuteCommandList() is called on a particular command
	// list, that command list can then be reset at any time and must be before
	// re-recording.
	ThrowIfFailed(m_commandList->Reset(m_CmdListAllocator.Get(), m_pipelineState.Get()));

	// Set necessary state.
	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Indicate that the back buffer will be used as a render target.
	auto s1 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &s1);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	m_commandList->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_commandList->DrawInstanced(3, 1, 0, 0);

	// Indicate that the back buffer will now be used to present.
	auto s2 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &s2);

	ThrowIfFailed(m_commandList->Close());
}











void Box::WaitForPreviousFrame()
{	
	const uint64_t fence = m_fenceValue;
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
	m_fenceValue++;
	if (m_fence->GetCompletedValue() < fence)
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}
