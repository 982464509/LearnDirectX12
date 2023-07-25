//***************************************************************************************
// @author : zxz
// @file :
// @description :ªÊ÷∆Box
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

}

void Box::OnDestroy()
{
	WaitForPreviousFrame();
	CloseHandle(m_fenceEvent);
}



void Box::LoadPipeline()
{

}
void Box::LoadAssets()
{

}
void Box::PopulateCommandList()
{

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
