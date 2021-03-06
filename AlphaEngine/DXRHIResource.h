#pragma once
#include "AResource.h"
class DXRHIResource : public ARHIResource
{
public:
	DXRHIResource();
	virtual void Release() override {};
	virtual std::shared_ptr<AResource> BackBuffer()override;

	virtual unsigned __int64  CurrentBackBufferViewHand()override;
	virtual unsigned __int64  CurrentDepthStencilViewHand()override;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
private:
	std::shared_ptr<AResource> mResource;
};

enum RESOURCE_STATES
{
	COMMON = 0,
	VERTEX_AND_CONSTANT_BUFFER = 0x1,
	INDEX_BUFFER = 0x2,
	RENDER_TARGET = 0x4,
	UNORDERED_ACCESS = 0x8,
	DEPTH_WRITE = 0x10,
	DEPTH_READ = 0x20,
	NON_PIXEL_SHADER_RESOURCE = 0x40,
	PIXEL_SHADER_RESOURCE = 0x80,
	STREAM_OUT = 0x100,
	INDIRECT_ARGUMENT = 0x200,
	COPY_DEST = 0x400,
	COPY_SOURCE = 0x800,
	RESOLVE_DEST = 0x1000,
	RESOLVE_SOURCE = 0x2000,
	RAYTRACING_ACCELERATION_STRUCTURE = 0x400000,
	SHADING_RATE_SOURCE = 0x1000000,
	RESOURCE_STATE_GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
	PRESENT = 0,
	PREDICATION = 0x200,
	VIDEO_DECODE_READ = 0x10000,
	VIDEO_DECODE_WRITE = 0x20000,
	VIDEO_PROCESS_READ = 0x40000,
	VIDEO_PROCESS_WRITE = 0x80000,
	VIDEO_ENCODE_READ = 0x200000,
	VIDEO_ENCODE_WRITE = 0x800000
};
