//***************************************************************************************
// @author : zxz
// @file :
// @description :	资源基类
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


