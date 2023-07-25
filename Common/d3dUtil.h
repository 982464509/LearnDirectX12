//***************************************************************************************
// d3dUtil.h by Frank Luna (C) 2015 All Rights Reserved.
//
// General helper code.
//***************************************************************************************

#pragma once

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "d3dx12.h"
#include "DDSTextureLoader.h"
#include "MathHelper.h"

extern const int gNumFrameResources;

inline void d3dSetDebugName(IDXGIObject* obj, const char* name)
{
    if(obj)
    {
        obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
    }
}
inline void d3dSetDebugName(ID3D12Device* obj, const char* name)
{
    if(obj)
    {
        obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
    }
}
inline void d3dSetDebugName(ID3D12DeviceChild* obj, const char* name)
{
    if(obj)
    {
        obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
    }
}

inline std::wstring AnsiToWString(const std::string& str)
{
    WCHAR buffer[512];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
}

class d3dUtil
{
public:

    static bool IsKeyDown(int vkeyCode);

    static std::string ToString(HRESULT hr);

	//�������Ĵ�С����ΪӲ����С����ռ䣨256B������������
    static UINT CalcConstantBufferByteSize(UINT byteSize)
    {
		// �����������Ĵ�С������Ӳ����С����ռ䣨ͨ����256B����������
		// Ϊ�ˣ�Ҫ�������Ϊ�����������С��256������������������ͨ��Ϊ����ֵbyteSize����255��
		// ��������ͽ���ĵ�2�ֽڣ�����������С��256�����ݲ��֣���ʵ����һ��
		// ���磺����byteSize = 300
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
        return (byteSize + 255) & ~255;
    }

	//��.cso�ļ����ѱ���õ���ɫ�������ֽ�����ص�Ӧ�ó�����
    static Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename);

    static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);
};


// ����SubmeshGeometry������MeshGeometry�д洢�ĵ���������
// �˽ṹ�������ڽ�������������ݴ���һ�����㻺������һ�����������������
// ���ṩ�˶Դ��ڶ��㻺�����������������еĵ�����������л�����������ݺ�ƫ���������ǿ��Ծݴ���
// ʵ��ͼ6.3�������ļ���
struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	// ͨ���������������嵱ǰSubmeshGeometry�ṹ�������漸����İ�Χ�У�bounding box��������
    // ���ڱ���ĺ����½���ʹ�ô�����
	DirectX::BoundingBox Bounds;
};

struct MeshGeometry
{	
	std::string Name;

	//CPU�ڴ��ϵ�����	
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;			
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr; 
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;
	
	
	// �뻺������ص�����
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;

	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	// һ��MeshGeometry�ṹ���ܹ��洢һ�鶥��/�����������еĶ��������
	// �������������������� �����񼸺��壬���Ǿ��ܵ����ػ��Ƴ����е������񣨵��������壩
	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

	//���㻺����������
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;
		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;
		return ibv;
	}

	// �������ϴ���GPU�����Ǿ����ͷ���Щ�ڴ��ˡ�
	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
	}
};

struct Light
{
    DirectX::XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };
    float FalloffStart = 1.0f;                          // point/spot light only
    DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };// directional/spot light only
    float FalloffEnd = 10.0f;                           // point/spot light only
    DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  // point/spot light only
    float SpotPower = 64.0f;                            // spot light only
};

#define MaxLights 16

struct MaterialConstants
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

struct Material
{
	// ���ڲ��Ҳ��ʵ�Ψһ��Ӧ����
	std::string Name;

	// �����ʵĳ�������������
	int MatCBIndex = -1;

	// ������������SRV���е��������ڵ�9��������ͼʱ���õ�
	int DiffuseSrvHeapIndex = -1;

	int NormalSrvHeapIndex = 0;

	// �Ѹ��±�־��dirty flag��Ҳ�����־����ʾ���������б䶯��������Ҳ����Ҫ���³����������ˡ�
	// ����ÿ��֡��ԴFrameResource����һ�����ʳ��������������Ա����ÿ��FrameResource����
	// �и��¡���ˣ����޸�ĳ������ʱ��Ӧ������NumFramesDirty = gNumFrameResources����ʹÿ
	// ��֡��Դ���ܵõ�����
	int NumFramesDirty = gNumFrameResources;

	// ������ɫ�Ĳ��ʳ�������������
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

struct Texture
{
	// Unique material name for lookup.
	std::string Name;

	std::wstring Filename;

	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
};

class DxException
{
public:
    DxException() = default;
    DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

    std::wstring ToString()const;

    HRESULT ErrorCode = S_OK;
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;
};

//ThrowIfFailed�ض���һ���꣬������һ��������
//������ˣ�__FILE__��__LINE__����λ��ThrowIfFailed���ڵ��ļ����У����ǳ��������ļ����С�
//L#x�Ὣ��ThrowIfFailed�Ĳ���ת��ΪUnicode�ַ���
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif