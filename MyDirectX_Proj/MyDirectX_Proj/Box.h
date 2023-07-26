#pragma once

#include "BaseDX.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;


struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class Box :public BaseDX
{
public:
	Box(uint32_t width, uint32_t height, std::wstring name);

private:
	
	void OnInit() override;
	void OnResize() override;
	void OnLoadAssets() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
};