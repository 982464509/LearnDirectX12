#pragma once

#include "../Base/BaseDX.h"

using namespace DirectX;

class ClearColor :public BaseDX
{
public:
	ClearColor(uint32_t width, uint32_t height, std::wstring name);

private:
	
	void OnInit() override;
	void OnResize() override;
	void OnLoadAssets() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
};