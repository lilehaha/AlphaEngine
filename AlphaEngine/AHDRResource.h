#pragma once
#include "stdafx.h"
#include "ARenderResource.h"

class AHDRResource : public ARenderResource
{
public:
	virtual void ReleaseResource() {};
	virtual std::shared_ptr<AResource> GetRTVResource(int index) = 0;
	virtual std::shared_ptr<AResource> GetDSVResource(int index) = 0;
	virtual unsigned __int64 SRV(int index) = 0;
	virtual unsigned __int64 DSV(int index) = 0;
	virtual unsigned __int64 RTV(int index) = 0;
	int HDRSize = 7;
	std::vector<int> width;
	std::vector<int> height;
	bool bIsBloomDown = false;
};

class DXHDRResource : public AHDRResource {
public:
	DXHDRResource();
	DXHDRResource(const DXHDRResource& rhs) = delete;
	DXHDRResource& operator=(const DXHDRResource& rhs) = delete;
	~DXHDRResource();

	virtual std::shared_ptr<AResource> GetRTVResource(int index) override;
	virtual std::shared_ptr<AResource> GetDSVResource(int index) override;
	virtual unsigned __int64 SRV(int index)override;
	virtual unsigned __int64 DSV(int index)override;
	virtual unsigned __int64 RTV(int index)override;

	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDsv,
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuRtv,
		int index,
		int width,
		int height
	);

private:
	void BuildDescriptors(int index);
	void BuildResource(int width, int height, int index);

	std::vector < CD3DX12_CPU_DESCRIPTOR_HANDLE > mhCpuSrv;
	std::vector < CD3DX12_CPU_DESCRIPTOR_HANDLE >mhCpuDsv;
	std::vector < CD3DX12_CPU_DESCRIPTOR_HANDLE >mhCpuRtv;
	std::shared_ptr<AResource> mResource;
	std::vector<ComPtr<ID3D12Resource>> mHDRs;
	std::vector <ComPtr<ID3D12Resource>> mShadows;

	int currentIndex = 0;
};