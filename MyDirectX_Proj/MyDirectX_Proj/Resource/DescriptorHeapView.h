

#include"DescriptorHeap.h"

class DescriptorHeapView
{
public:	
	DescriptorHeapView(DescriptorHeap const* heap, UINT64 index)				
		: heap(heap),
		index(index)
	{}

	DescriptorHeapView(DescriptorHeap const* heap)		
		: heap(heap),
		index(0)
	{}

	DescriptorHeap const* heap;
	UINT64 index;
};
