#pragma once
#include <d3d12.h>


class CommandListHandle
{
public:		
	CommandListHandle(
		ID3D12CommandAllocator* allocator,
		ID3D12GraphicsCommandList* cmdList);
	CommandListHandle(CommandListHandle&&);
	CommandListHandle(CommandListHandle const&) = delete;

	~CommandListHandle();

	ID3D12GraphicsCommandList* CmdList() const
	{
		return cmdList;
	}
private:
	ID3D12GraphicsCommandList* cmdList;
};