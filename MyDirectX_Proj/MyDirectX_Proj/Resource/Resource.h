#pragma once

#include "../RunTime/Device.h"


class Resource
{
protected:
	Device* device;


public:
	Resource(Device* device) : device(device) {}
	Resource(Resource&&) = default;
	Resource(Resource const&) = delete;
	virtual ~Resource() = default;


	Device* GetDevice() const { return device; }

	virtual ID3D12Resource* GetResource() const
	{
		return nullptr;
	}

	virtual D3D12_RESOURCE_STATES GetInitState() const
	{
		return D3D12_RESOURCE_STATE_COMMON;
	}

	// virtual void DelayDispose(FrameResource* frameRes) const {}

};
