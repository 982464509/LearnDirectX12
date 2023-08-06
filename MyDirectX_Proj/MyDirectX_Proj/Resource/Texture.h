#pragma once

#include"Resource.h"

enum class TextureType:UINT16
{
	None,
	Tex1D,
	Tex2D,
	Tex3D,
	Cubemap,
	Tex2DArray,
};

class Texture final : public Resource
{
public:
	enum class TextureUsage : UINT16
	{
		None = 0,
		RenderTarget = 0x1,
		DepthStencil = 0x2,
		UnorderedAccess = 0x4,
		GenericColor = (0x4 | 0x1) // Both render target and unordered access
	};
	static constexpr float CLEAR_COLOR[4] = { 0,0,0,0 };
	static constexpr float CLEAR_DEPTH = 1;
	static constexpr uint8_t CLEAR_STENCIL = 0;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_RESOURCE_STATES initState;
	TextureType textureType;
	TextureUsage usage;

public:
	Texture(Device* device, UINT width, UINT height, DXGI_FORMAT format, 
		TextureType textureType, UINT depth, UINT mip, TextureUsage usage, 
		D3D12_RESOURCE_STATES resourceState);											
	Texture(Device* device, IDXGISwapChain3* swapchain, UINT frame);						
	~Texture();

	ID3D12Resource* GetResource() const override
	{
		return resource.Get();
	}
	D3D12_RESOURCE_STATES GetInitState() const
	{
		return initState;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC GetColorSrvDesc(UINT mipOffset) const;
	D3D12_UNORDERED_ACCESS_VIEW_DESC GetColorUavDesc(UINT targetMipLevel) const;
	// void DelayDispose(FrameResource* frameRes) const override;

};