#pragma once
#include "../Base/BaseDX.h"
#include"../Utility/MathHelper.h"
#include"../Resource/UploadBuffer.h"
#include"../Resource/MaterialFrameResource.h"
#include"../Utility/GeometryGenerator.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

struct MaterialRenderItem
{
	MaterialRenderItem() = default;

	XMFLOAT4X4 World = MathHelper::Identity4x4();
	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	int NumFramesDirty = gNumFrameResources;

	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;			//索引起始
	int BaseVertexLocation = 0;			//顶点起始
};

class LitColumns :public BaseDX
{
public:
	LitColumns(uint32_t width, uint32_t height, std::wstring name);

private:
	std::vector<std::unique_ptr<MaterialFrameResource>> mFrameResources;
	MaterialFrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
	
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::vector<std::unique_ptr<MaterialRenderItem>> mAllRitems;
	std::vector<MaterialRenderItem*> mOpaqueRitems;

	LightPassConstants mMainPassCB;

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.2f * XM_PI;
	float mRadius = 15.0f;

	POINT mLastMousePos;

	bool mIsWireframe;

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
	void BuildShapeGeometry();
	void BuildSkullGeometry();	
	void BuildRenderItems();
	void BuildPSOs();
	void BuildMaterials();
	void BuildGeometry(
		std::string name,
		std::vector<PNVertex>* vertices,
		std::vector<std::int32_t>* indices,
		std::unordered_map<std::string, SubmeshGeometry>* subMeshMap);
};
