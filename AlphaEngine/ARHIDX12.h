#pragma once

#include "ARHI.h"
#include "UploadBuffer.h"
#include "ARenderScene.h"

#include "CommonResourceDefine.h"
using namespace DirectX;
using namespace DirectX::PackedVector;


class ARHIDX12 : public ARHI
{
public:
	ARHIDX12();
	virtual ~ARHIDX12();
	virtual bool Init() override;
	virtual void OnResize();
	virtual void Draw(std::shared_ptr<ARenderScene> RenderScene) override;
	virtual void Update(int CBIndex, ARenderItem* renderItem) override;

	void SetWindow(HWND HWnd);
	float AspectRatio() const;
private:
	bool InitD3D();
	virtual void CreateRtvAndDsvDescriptorHeaps();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void CreateCommandObjects();
	void CreateSwapChain();

	void FlushCommandQueue();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


	void BuildDescriptorHeaps();
	void BuildConstantBuffers(ARenderItem* renderItem);
	void BuildShaderResourceView(const std::string& ActorName, ARenderItem* RenderItem, const std::string& MeshName, std::shared_ptr<ARenderScene> RenderScene);
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildPSO();

	void SetGraphicsRootDescriptorTable(ARenderItem* RenderItem);
	void SetGraphicsRoot32BitConstants(int Width, int Height);

	void BuildRenderItem(std::shared_ptr<ARenderScene> sceneResource);
public:
	virtual void RenderFrameBegin(std::shared_ptr<ARenderScene> RenderScene) override;
	virtual void CreateCbHeapsAndSrv(const std::string& ActorName, const std::string& MeshName, ARenderItem* RenderItem, std::shared_ptr<ARenderScene> RenderScene) override;
	virtual void ResetCommand(const std::string& PSOName) override;
	virtual void ExecuteCommandLists() override;
	virtual void CreateTextureResource(std::shared_ptr<ARenderScene> RenderScene, std::shared_ptr<ATexture> Texture) override;
private:
	HWND mhMainWnd = nullptr;

	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;      // quality level of 4X MSAA

	ComPtr<IDXGIFactory4> mdxgiFactory;
	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource> mDepthStencilBuffer;

	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth;
	int mClientHeight;

	ComPtr<ID3D12DescriptorHeap> mCbvSrvHeap = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	std::unique_ptr<UploadBuffer<MatConstants>> mMatCB = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mPSO = nullptr;

	glm::mat4x4 mWorld = glm::identity<glm::mat4x4>();
	glm::mat4x4 mView = glm::identity<glm::mat4x4>();
	glm::mat4x4 mProj = glm::identity<glm::mat4x4>();

	int mHeapIndex = 0;
	int mEleIndex = 0;

	glm::vec3 mCameraLoc;
};

