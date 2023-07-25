#pragma once
#include "d3dUtil.h"

//1. �ϴ���������Դ�Ĺ�������������
//2. ������Դ��ӳ���ȡ��ӳ�����
//3. CopyData���������»������ڵ��ض�Ԫ��

template<typename T>
class UploadBuffer
{
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
    BYTE* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;

public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : 
        mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);

        // �����������Ĵ�СΪ256B����������������ΪӲ��ֻ�ܰ�m*256B��ƫ������n*256B�����ݳ��������ֹ�����鿴��������
        // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // D3D12_GPU_VIRTUAL_ADDRESS BufferLocation; // 256��������
        // UINT  SizeInBytes;}     // 256��������       
        if (isConstantBuffer) 
        {
            mElementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(T));
        }            
        auto heap = &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        device->CreateCommittedResource(heap, D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount)
            , D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mUploadBuffer));
            
        mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
        // ֻҪ�����޸ĵ�ǰ����Դ�����Ǿ�����ȡ��ӳ��
        // ���ǣ�����Դ��GPUʹ���ڼ䣬����ǧ�򲻿������Դ����д���������Ա��������ͬ��������
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

    ~UploadBuffer()
    {
        if(mUploadBuffer != nullptr)
            mUploadBuffer->Unmap(0, nullptr);
        mMappedData = nullptr;
    }

    ID3D12Resource* Resource()const
    {
        return mUploadBuffer.Get();
    }

    /// <summary>
    /// �����ݴ�ϵͳ�ڴ渴�Ƶ�����������
    /// </summary>
    /// <param name="elementIndex"></param>
    /// <param name="data"></param>
    void CopyData(int elementIndex, const T& data)
    {        
        memcpy(&mMappedData[elementIndex*mElementByteSize], &data, sizeof(T));
    }
};