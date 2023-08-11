#pragma once
#include "../Base/BaseDX.h"
#include"../Utility/MathHelper.h"
#include"../Resource/UploadBuffer.h"
#include"../Resource/FrameResource.h"
#include"../Utility/GeometryGenerator.h"


using namespace DirectX;


struct RenderItem
{
    RenderItem() = default;

    XMFLOAT4X4 World = MathHelper::Identity4x4();
    UINT NumFramesDirty = gNumFrameResources;

    UINT ObjCBIndex = -1;

    MeshGeometry* Geo = nullptr;

    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    int BaseVertexLocation = 0;
};


class Shapes :public BaseDX
{
public:
    Shapes(uint32_t width, uint32_t height, std::wstring name);

private:
    std::vector<std::unique_ptr<FrameResource>> mFrameResources;
    FrameResource* mCurrFrameResource = nullptr;
    int mCurrFrameResourceIndex = 0;


    std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;

    std::vector<std::unique_ptr<RenderItem>> mAllRitems;//  存有所有渲染项
    std::vector<RenderItem*> mOpaqueRitems; //根据PSO来划分渲染项

    std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    bool mIsWireframe = false;

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f*XM_PI;
    float mPhi = 0.2f*XM_PI;
    float mRadius = 15.0f;

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


    void BuildRootSignature();
    void BuildShapesGeometry();
    void BuildRenderItems();
    void BuildPSOs();
};
