//***************************************************************************************
// @author : zxz
// @file :
// @description :	��Դ����
//***************************************************************************************

#include "Buffer.h"

Buffer::Buffer(
	Device* device)
	: Resource(device)
{
}
Buffer::~Buffer()
{
}

UINT64 Buffer::GetByteSize()const
{
	return bufferByteSize;
}


