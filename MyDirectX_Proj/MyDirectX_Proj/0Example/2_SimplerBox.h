#pragma once

#include "../Base/BaseDX.h"


using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ObjectConstants
{
	// XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    XMFLOAT2 ColorW = DirectX::XMFLOAT2(1,1);
};

class SimplerBox :public BaseDX
{
public:
	SimplerBox(uint32_t width, uint32_t height, std::wstring name);

private:
    /*std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

    Microsoft::WRL::ComPtr<ID3DBlob> mvsByteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> mpsByteCode = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();*/

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    POINT mLastMousePos;

    virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	void OnInit() override;
	void OnResize() override;
	void OnLoadAssets() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

    void BuildBoxGeometry();
};
