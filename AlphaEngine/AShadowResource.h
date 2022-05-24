#pragma once
#include "stdafx.h"
#include "ARenderResource.h"
#include "AResource.h"

class AShadowResource : public ARenderResource
{
public:
	virtual void ReleaseResource() {};
	virtual std::shared_ptr<AResource> GetResource() = 0;
	virtual unsigned __int64 SRV() = 0;
	virtual unsigned __int64 DSV() = 0;
};


class DXShadowResource :public AShadowResource {
public:
	DXShadowResource();
	~DXShadowResource();

	virtual std::shared_ptr<AResource> GetResource() override;
	virtual unsigned __int64 SRV() override;
	virtual unsigned __int64 DSV()override;

	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDsv
	);
private:
	void BuildResource();

	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuDsv;
	std::shared_ptr<AResource> mResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> mShadowMap = nullptr;
};
