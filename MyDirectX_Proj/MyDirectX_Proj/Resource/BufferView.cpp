//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "BufferView.h"


BufferView::BufferView(Buffer const* buffer)
	: buffer(buffer),
	byteSize(buffer ? buffer->GetByteSize() : 0),
	offset(0)
{
}

BufferView::BufferView(Buffer const* buffer, UINT64 offset, UINT64 byteSize)			
	: buffer(buffer),
	offset(offset),
	byteSize(byteSize)
{
}