#include "stdafx.h"
#include "ARHIDX12.h"
#include "AEngine.h"
#include "AShadowResource.h"
#include "AHDRResource.h"

ARHIDX12::ARHIDX12()
{
	SetWindow(AEngine::GetSingleton().GetWindow()->GetHWND());
	mClientWidth = AEngine::GetSingleton().GetWindow()->GetWidth();
	mClientHeight = AEngine::GetSingleton().GetWindow()->GetHeight();
}

ARHIDX12::~ARHIDX12()
{

}

bool ARHIDX12::Init()
{
	if (!InitD3D())
	{
		return false;
	}

	OnResize();

	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1000, true);
	mMatCB = std::make_unique<UploadBuffer<MatConstants>>(md3dDevice.Get(), 1000, true);

	BuildDescriptorHeaps();

	return true;
}

void ARHIDX12::OnResize()
{
	assert(md3dDevice);
	assert(mSwapChain);
	assert(mDirectCmdListAlloc);

	FlushCommandQueue();

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	for (int i = 0; i < SwapChainBufferCount; ++i)
		mSwapChainBuffer[i].Reset();
	mDepthStencilBuffer.Reset();

	ThrowIfFailed(mSwapChain->ResizeBuffers(
		SwapChainBufferCount,
		mClientWidth, mClientHeight,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	mCurrBackBuffer = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	ThrowIfFailed(md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };

	AEngine::GetSingleton().GetScene()->GetCamera()->SetACameraPosition(500.0f, 500.0f, 500.0f);
	AEngine::GetSingleton().GetScene()->GetCamera()->SetLens(0.25f * glm::pi<float>(), AspectRatio(), 1.0f, 10000.0f);
	AEngine::GetSingleton().GetScene()->GetCamera()->LookAt(AEngine::GetSingleton().GetScene()->GetCamera()->GetACameraPosition(),
		glm::vec3(0.0f, 0.0f, 0.0f), AEngine::GetSingleton().GetScene()->GetCamera()->GetUp());
}

void ARHIDX12::Draw(std::shared_ptr<ARenderItem> renderItem, const std::string& renderItemName, bool IsDrawDepth, bool isNeedRTV, int RTVNumber, int width, int height)
{
	IASetVertexAndIndexBuffers(CreateBuffer(renderItem, renderItemName));
	IASetPrimitiveTopology();
	SetGraphicsRootDescriptorTable(renderItem.get(), IsDrawDepth, isNeedRTV, RTVNumber, width, height);
	DrawIndexedInstanced(renderItem, renderItemName);
}

void ARHIDX12::Update(int CBIndex, std::shared_ptr<ARenderScene> RenderScene, ARenderItem* renderItem)
{
	mCameraLoc = AEngine::GetSingleton().GetScene()->GetCamera()->GetACameraPosition();
	AEngine::GetSingleton().GetScene()->GetCamera()->UpdateViewMatrix();

	glm::mat4x4 proj = AEngine::GetSingleton().GetScene()->GetCamera()->GetProjMatrix();
	glm::mat4x4 view = AEngine::GetSingleton().GetScene()->GetCamera()->GetViewMatrix();

	MatConstants matContants;
	mMatCB->CopyData(CBIndex, matContants);

	ObjectConstants objConstants;
	objConstants.world = renderItem->mWorld;
	objConstants.viewProj = glm::transpose(proj * view);
	objConstants.Rotation = renderItem->mRotation;
	objConstants.LightVP = RenderScene->LightVP;
	objConstants.TLightVP = RenderScene->TLightVP;
	objConstants.directionalLight.Location = AEngine::GetSingleton().GetScene()->DirectionalLight.Location;
	objConstants.directionalLight.Direction = RenderScene->LightDirection;
	objConstants.directionalLight.Brightness = AEngine::GetSingleton().GetScene()->DirectionalLight.Brightness;
	objConstants.CameraLoc = mCameraLoc;
	objConstants.Time = AEngine::GetSingleton().GetTotalTime();
	mObjectCB->CopyData(CBIndex, objConstants);
}

void ARHIDX12::SetWindow(HWND HWnd)
{
	mhMainWnd = HWnd;
}

float ARHIDX12::AspectRatio() const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}

ComPtr<ID3D12Device> ARHIDX12::GetDevice()
{
	return md3dDevice;
}

bool ARHIDX12::InitD3D()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice));

	// If create hardware adapter failed, Fallback to WARP device.
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice)));
	}

	ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence)));

	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Check 4X MSAA quality support for our back buffer format.
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

#ifdef _DEBUG
	LogAdapters();
#endif
	CreateCommandObjects();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();

	return true;
}

void ARHIDX12::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = 100;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 10;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
}

ID3D12Resource* ARHIDX12::CurrentBackBuffer() const
{
	return mSwapChainBuffer[mCurrBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE ARHIDX12::CurrentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE ARHIDX12::DepthStencilView() const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void ARHIDX12::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	ThrowIfFailed(md3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())));

	ThrowIfFailed(md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(mCommandList.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	mCommandList->Close();
}

void ARHIDX12::CreateSwapChain()
{
	mSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(mdxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		mSwapChain.GetAddressOf()));
}

void ARHIDX12::FlushCommandQueue()
{
	mCurrentFence++;

	ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));

	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void ARHIDX12::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (mdxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void ARHIDX12::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, mBackBufferFormat);

		ReleaseCom(output);

		++i;
	}
}

void ARHIDX12::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}

void ARHIDX12::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvsrvHeapDesc;
	cbvsrvHeapDesc.NumDescriptors = 1000;
	cbvsrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvsrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvsrvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvsrvHeapDesc,
		IID_PPV_ARGS(&mCbvSrvHeap)));
}

void ARHIDX12::BuildConstantBuffers(ARenderItem* renderItem)
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();

	cbAddress += mEleIndex * objCBByteSize;
	auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	handle.Offset(mHeapIndex, mCbvSrvUavDescriptorSize);
	renderItem->mCBHeapIndex = mHeapIndex;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = objCBByteSize;

	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		handle);

	UINT matConstSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	D3D12_GPU_VIRTUAL_ADDRESS matCB_Address;
	matCB_Address = mMatCB->Resource()->GetGPUVirtualAddress();
	matCB_Address += mEleIndex * matConstSize;

	mHeapIndex++;
	handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	handle.Offset(mHeapIndex, mCbvSrvUavDescriptorSize);
	renderItem->mMatCBIndex = mHeapIndex;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc1;
	cbvDesc1.BufferLocation = matCB_Address;
	cbvDesc1.SizeInBytes = matConstSize;
	md3dDevice->CreateConstantBufferView(&cbvDesc1, handle);

	mEleIndex++;
	//mHeapIndex++;
}

void ARHIDX12::BuildShaderResourceView(const std::string& ActorName, ARenderItem* RenderItem, const std::string& MeshName, ARenderResource* RenderResource, ARenderResource* HDRResource, std::shared_ptr<ARenderScene> RenderScene)
{
	RenderItem->mSrvCBIndex = ++mHeapIndex;
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	hDescriptor.Offset(mHeapIndex, mCbvSrvUavDescriptorSize);
	std::string ResourceName;
	std::string name(MeshName.c_str());
	name.resize(name.size());

	ComPtr<ID3D12Resource>  DiffuseTex;
	ComPtr<ID3D12Resource>  NormalTex;
	if (RenderScene->mDiffuseTexs.find(name) == RenderScene->mDiffuseTexs.end())
	{
		ResourceName = "Null";
	}
	else
	{
		ResourceName = name;
	}
	DiffuseTex = RenderScene->mDiffuseTexs[ResourceName]->mResource;
	if (RenderScene->mNormalTexs.find(name) == RenderScene->mNormalTexs.end())
	{
		ResourceName = "Null";
	}
	else
	{
		ResourceName = name;
	}
	NormalTex = RenderScene->mNormalTexs[ResourceName]->mResource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DiffuseTex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = DiffuseTex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	md3dDevice->CreateShaderResourceView(DiffuseTex.Get(), &srvDesc, hDescriptor);

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor2(mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	hDescriptor2.Offset(++mHeapIndex, mCbvSrvUavDescriptorSize);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2 = {};
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.Format = NormalTex->GetDesc().Format;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MostDetailedMip = 0;
	srvDesc2.Texture2D.MipLevels = NormalTex->GetDesc().MipLevels;
	srvDesc2.Texture2D.ResourceMinLODClamp = 0.0f;
	md3dDevice->CreateShaderResourceView(NormalTex.Get(), &srvDesc2, hDescriptor2);

	auto srvCpuStart = mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart();
	auto dsvCpuStart = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	dynamic_cast<DXShadowResource*>(RenderResource)->BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(srvCpuStart, ++mHeapIndex, mCbvSrvUavDescriptorSize),
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvCpuStart, 1, mDsvDescriptorSize));
	auto rtvCpuStart = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	//HDR
	for (int i = 0; i < dynamic_cast<DXHDRResource*>(HDRResource)->HDRSize; i++)
	{
		float scale = 1.0f;

		if (i >= 1) {
			scale = 0.25 * static_cast<float>(glm::pow(0.5, glm::min(i - 1, 3)));
		}

		if (i >= 4) {
			scale = scale * static_cast<float>(glm::pow(2, i - 2));
		}
		if (scale > 0.25) {
			scale = 1;
		}
		dynamic_cast<DXHDRResource*>(HDRResource)->BuildDescriptors(
			CD3DX12_CPU_DESCRIPTOR_HANDLE(srvCpuStart, ++mHeapIndex, mCbvSrvUavDescriptorSize),
			CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvCpuStart, 2, mDsvDescriptorSize),
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvCpuStart, i + 2, mRtvDescriptorSize),
			i, static_cast<int>(1920 * scale), static_cast<int>(1080 * scale));

		RenderItem->ObjRtvIndex.push_back(mHeapIndex);
	}
	mHeapIndex++;
}

void ARHIDX12::BuildRootSignature(AShader* shader)
{
	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		shader->mvsByteCode->GetBufferPointer(),
		shader->mvsByteCode->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}

void ARHIDX12::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	mvsByteCode = d3dUtil::CompileShader(L"..\\AlphaEngine\\Shaders\\Color.hlsl", nullptr, "VS", "vs_5_0");
	mpsByteCode = d3dUtil::CompileShader(L"..\\AlphaEngine\\Shaders\\Color.hlsl", nullptr, "PS", "ps_5_0");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void ARHIDX12::ClearRenderTargetView(unsigned __int64 ptr)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle;
	handle.ptr = ptr;
	mCommandList->ClearRenderTargetView(handle, Colors::LightSteelBlue, 0, nullptr);
}

void ARHIDX12::ClearDepthStencilView(unsigned __int64 ptr)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle;
	handle.ptr = ptr;
	mCommandList->ClearDepthStencilView(handle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void ARHIDX12::OMSetStencilRef(int StencilRef)
{
	mCommandList->OMSetStencilRef(StencilRef);
}

void ARHIDX12::OMSetRenderTargets(int numTatgetDescriptors, unsigned __int64 RTptr, bool RTsSingleHandleToDescriptorRange, unsigned __int64 DSptr)
{
	std::unique_ptr<CD3DX12_CPU_DESCRIPTOR_HANDLE> RThandle = std::make_unique<CD3DX12_CPU_DESCRIPTOR_HANDLE>();
	std::unique_ptr<CD3DX12_CPU_DESCRIPTOR_HANDLE> DShandle = std::make_unique<CD3DX12_CPU_DESCRIPTOR_HANDLE>();

	if (RTptr != 0) {
		RThandle->ptr = RTptr;
	}
	else
	{
		RThandle = nullptr;
	}

	if (DSptr != 0) {
		DShandle->ptr = DSptr;
	}
	else
	{
		DShandle = nullptr;
	}
	mCommandList->OMSetRenderTargets(numTatgetDescriptors, RThandle.get(), RTsSingleHandleToDescriptorRange, DShandle.get());
}

void ARHIDX12::SetDescriptorHeaps()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void ARHIDX12::SetGraphicsRootSignature()
{
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
}

void ARHIDX12::IASetVertexAndIndexBuffers(ABuffer* buffer)
{
	auto dxBuffer = dynamic_cast<DXBuffer*>(buffer);

	mCommandList->IASetVertexBuffers(0, 1, &dxBuffer->VertexBufferView());
	mCommandList->IASetIndexBuffer(&dxBuffer->IndexBufferView());
}

void ARHIDX12::IASetPrimitiveTopology()
{
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ARHIDX12::BuildPSO(std::shared_ptr<ARenderItem> renderItem, AMaterial Mat)
{
	if (PSONames.find(Mat.mPSO.Name) != PSONames.end()) {
		return;
	}
	BuildRootSignature(AShaderManager::GetSingleton().CompileShader(Mat.mShaderFilePath));
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PSOState;
	Mat.mPSO.PipelineState.pRootSignature = mRootSignature.Get();
	Mat.mPSO.PipelineState.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	Mat.mPSO.PipelineState.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	PSOState = Mat.mPSO.PipelineState;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&PSOState, IID_PPV_ARGS(&mPSO[Mat.mPSO.Name])));
	PSONames.insert(Mat.mPSO.Name);
}

void ARHIDX12::ChangePSOState(AMaterial Mat, const APSO& PSO, const std::wstring& Shader)
{
	Mat.mShaderFilePath = Shader;
	Mat.mPSO = PSO;
}

void ARHIDX12::ClearAndSetRenderTatget(unsigned __int64 ClearRenderTargetHand, unsigned __int64 ClearDepthStencilHand, int numTatgetDescriptors, unsigned __int64 SetRenderTargetHand, bool RTsSingleHandleToDescriptorRange, unsigned __int64 SetDepthStencilHand)
{
	if (ClearRenderTargetHand != 0) {
		ClearRenderTargetView(ClearRenderTargetHand);
	}
	if (ClearDepthStencilHand != 0) {
		ClearDepthStencilView(ClearDepthStencilHand);
	}
	OMSetRenderTargets(numTatgetDescriptors, SetRenderTargetHand, RTsSingleHandleToDescriptorRange, SetDepthStencilHand);
	SetDescriptorHeaps();
	SetGraphicsRootSignature();
}

void ARHIDX12::SetGraphicsRootDescriptorTable(ARenderItem* renderItem, bool isDepth, bool isNeedRTV, int RTVNumber, int width, int height)
{
	SetGraphicsRoot32BitConstants(width, height);

	auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
	handle.Offset(renderItem->mCBHeapIndex, mCbvSrvUavDescriptorSize);
	mCommandList->SetGraphicsRootDescriptorTable(1, handle);
	if (!isDepth)
	{
		auto handle2 = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
		handle2.Offset(renderItem->mSrvCBIndex, mCbvSrvUavDescriptorSize);
		mCommandList->SetGraphicsRootDescriptorTable(2, handle2);
	}
	if (isNeedRTV)
	{
		for (int i = 0; i < RTVNumber; i++)
		{
			int currentRTVNumber = 0;
			CD3DX12_GPU_DESCRIPTOR_HANDLE hDescriptor3(mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
			hDescriptor3.Offset(renderItem->ObjRtvIndex[i], md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
			currentRTVNumber = i;
			if (i < 4) {
				currentRTVNumber = glm::min(RTVNumber, 4) - i;
				if (RTVNumber > 4 && i != 3) {
					currentRTVNumber--;
				}
			}
			else if (i >= 4) {
				currentRTVNumber = 1;
			}
			if (i == 0 || i > 5) {
				currentRTVNumber = 0;
			}
			mCommandList->SetGraphicsRootDescriptorTable(currentRTVNumber + 3, hDescriptor3);
		}
	}
}

void ARHIDX12::SetGraphicsRoot32BitConstants(int Width, int Height)
{
	int renderTargetSize[2] = { Width,Height };
	mCommandList->SetGraphicsRoot32BitConstants(0, 2, &renderTargetSize, 0);
}

void ARHIDX12::DrawIndexedInstanced(std::shared_ptr<ARenderItem> renderItem, const std::string& Name)
{
	mCommandList->DrawIndexedInstanced(renderItem->mGeo->DrawArgs[Name].IndexCount, 1,
		(UINT)renderItem->mGeo->DrawArgs[Name].StartIndexLocation,
		(UINT)renderItem->mGeo->DrawArgs[Name].BaseVertexLocation, 0);
}

void ARHIDX12::BuildRenderItem(std::shared_ptr<ARenderScene> sceneResource, const std::string& MatName)
{
	std::unordered_map<std::string, MeshData> meshDataMap = sceneResource->BuildMeshData();

	for (const auto& meshDataPair : meshDataMap)
	{
		if (sceneResource->mRenderItem[meshDataPair.first] == nullptr) {
			sceneResource->mRenderItem[meshDataPair.first] = std::make_shared<ARenderItem>();
		}
		if (sceneResource->mRenderItem[meshDataPair.first]->mGeo == nullptr) {
			sceneResource->mRenderItem[meshDataPair.first]->mGeo = std::make_unique<DXBuffer>();
		}
		sceneResource->mRenderItem[meshDataPair.first]->MatName = MatName;
		const UINT vbByteSize = (UINT)meshDataPair.second.Vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)meshDataPair.second.Indices.size() * sizeof(uint32_t);
		sceneResource->mRenderItem[meshDataPair.first]->mGeo->Name = meshDataPair.first;
		ThrowIfFailed(D3DCreateBlob(vbByteSize, &sceneResource->mRenderItem[meshDataPair.first]->mGeo->VertexBufferCPU));
		CopyMemory(sceneResource->mRenderItem[meshDataPair.first]->mGeo->VertexBufferCPU->GetBufferPointer(), meshDataPair.second.Vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &sceneResource->mRenderItem[meshDataPair.first]->mGeo->IndexBufferCPU));
		CopyMemory(sceneResource->mRenderItem[meshDataPair.first]->mGeo->IndexBufferCPU->GetBufferPointer(), meshDataPair.second.Indices.data(), ibByteSize);

		sceneResource->mRenderItem[meshDataPair.first]->mGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), meshDataPair.second.Vertices.data(), vbByteSize, sceneResource->mRenderItem[meshDataPair.first]->mGeo->VertexBufferUploader);
		sceneResource->mRenderItem[meshDataPair.first]->mGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), meshDataPair.second.Indices.data(), ibByteSize, sceneResource->mRenderItem[meshDataPair.first]->mGeo->IndexBufferUploader);

		sceneResource->mRenderItem[meshDataPair.first]->mGeo->VertexByteStride = sizeof(Vertex);
		sceneResource->mRenderItem[meshDataPair.first]->mGeo->VertexBufferByteSize = vbByteSize;
		sceneResource->mRenderItem[meshDataPair.first]->mGeo->IndexFormat = DXGI_FORMAT_R32_UINT;
		sceneResource->mRenderItem[meshDataPair.first]->mGeo->IndexBufferByteSize = ibByteSize;

		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT)meshDataPair.second.Indices.size();
		submesh.StartIndexLocation = sceneResource->mRenderItem[meshDataPair.first]->StartIndexLocation;
		submesh.BaseVertexLocation = sceneResource->mRenderItem[meshDataPair.first]->BaseVertexLocation;

		sceneResource->mRenderItem[meshDataPair.first]->mGeo->DrawArgs[meshDataPair.first] = submesh;
	}
	//Build HDRTriangle
	if (sceneResource->HDRTriangle == nullptr) {
		sceneResource->HDRTriangle = std::make_shared<ARenderItem>();
	}
	if (sceneResource->HDRTriangle->mGeo == nullptr) {
		sceneResource->HDRTriangle->mGeo = std::make_unique<DXBuffer>();
	}
	sceneResource->HDRTriangle->MatName = MatName;
	const UINT vbByteSize = (UINT)sceneResource->HDRGeo->Vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)sceneResource->HDRGeo->Indices.size() * sizeof(uint32_t);
	sceneResource->HDRTriangle->mGeo->Name = "HDRTriangle";
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &sceneResource->HDRTriangle->mGeo->VertexBufferCPU));
	CopyMemory(sceneResource->HDRTriangle->mGeo->VertexBufferCPU->GetBufferPointer(), sceneResource->HDRGeo->Vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &sceneResource->HDRTriangle->mGeo->IndexBufferCPU));
	CopyMemory(sceneResource->HDRTriangle->mGeo->IndexBufferCPU->GetBufferPointer(), sceneResource->HDRGeo->Indices.data(), ibByteSize);

	sceneResource->HDRTriangle->mGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), sceneResource->HDRGeo->Vertices.data(), vbByteSize, sceneResource->HDRTriangle->mGeo->VertexBufferUploader);
	sceneResource->HDRTriangle->mGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), sceneResource->HDRGeo->Indices.data(), ibByteSize, sceneResource->HDRTriangle->mGeo->IndexBufferUploader);

	sceneResource->HDRTriangle->mGeo->VertexByteStride = sizeof(Vertex);
	sceneResource->HDRTriangle->mGeo->VertexBufferByteSize = vbByteSize;
	sceneResource->HDRTriangle->mGeo->IndexFormat = DXGI_FORMAT_R32_UINT;
	sceneResource->HDRTriangle->mGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)sceneResource->HDRGeo->Indices.size();

	submesh.StartIndexLocation = sceneResource->HDRTriangle->StartIndexLocation;
	submesh.BaseVertexLocation = sceneResource->HDRTriangle->BaseVertexLocation;

	sceneResource->HDRTriangle->mGeo->DrawArgs["HDRTriangle"] = submesh;
}

void ARHIDX12::RenderFrameBegin(std::shared_ptr<ARenderScene> renderResource, const std::string& MatName)
{
	BuildRenderItem(renderResource, MatName);
}

void ARHIDX12::CreateCbHeapsAndSrv(const std::string& ActorName, const std::string& MeshName, ARenderItem* RenderItem, ARenderResource* shadowResource, ARenderResource* HDRResource, std::shared_ptr<ARenderScene> RenderScene)
{
	BuildConstantBuffers(RenderItem);
	BuildShaderResourceView(ActorName, RenderItem, MeshName, shadowResource, HDRResource, RenderScene);
}

void ARHIDX12::ResetCommand(const std::string& PSOName)
{
	if (PSOName == "Null") {
		ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
	}
	else
	{
		ThrowIfFailed(mDirectCmdListAlloc->Reset());
		ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO[PSOName].Get()));
	}
}

void ARHIDX12::RSSetViewports(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth)
{
	mScreenViewport.Height = Height;
	mScreenViewport.MaxDepth = MaxDepth;
	mScreenViewport.MinDepth = MinDepth;
	mScreenViewport.TopLeftX = TopLeftX;
	mScreenViewport.TopLeftY = TopLeftY;
	mScreenViewport.Width = Width;
	mCommandList->RSSetViewports(1, &mScreenViewport);
}

void ARHIDX12::RSSetScissorRects(long left, long top, long right, long bottom)
{
	mScissorRect.bottom = bottom;
	mScissorRect.right = right;
	mScissorRect.left = left;
	mScissorRect.top = top;
	mCommandList->RSSetScissorRects(1, &mScissorRect);
}

void ARHIDX12::ResourceBarrier(unsigned int NumberBarrier, std::shared_ptr<AResource> Resource, int stateBefore, int stateAfter)
{
	auto afterState = D3D12_RESOURCE_STATES(stateAfter);
	mCommandList->ResourceBarrier(NumberBarrier, &CD3DX12_RESOURCE_BARRIER::Transition(Resource->Resource, D3D12_RESOURCE_STATES(stateBefore), D3D12_RESOURCE_STATES(afterState)));
}

void ARHIDX12::SetPipelineState(std::shared_ptr<ARenderItem> renderItem, AMaterial Mat)
{
	BuildPSO(renderItem, Mat);
	if (Mat.mPSO.Name != currentPSOName) {
		mCommandList->SetPipelineState(mPSO[Mat.mPSO.Name].Get());
	}
	else {
		return;
	}
	currentPSOName = Mat.mPSO.Name;
}

void ARHIDX12::ExecuteCommandLists()
{
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
	FlushCommandQueue();
	currentPSOName = "";
}

void ARHIDX12::CreateTextureResource(std::shared_ptr<ARenderScene> RenderScene, std::shared_ptr<ATexture> Texture)
{
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice.Get(), mCommandList.Get(), Texture->mFilePath.c_str(), Texture->mResource, Texture->mUploadHeap));

	switch (Texture->mType)
	{
	case E_TexType::Diffuse:
		RenderScene->mDiffuseTexs[Texture->mName] = std::move(Texture);
		break;
	case E_TexType::Normal:
		RenderScene->mNormalTexs[Texture->mName] = std::move(Texture);
		break;
	default:
		assert(0);
		break;
	}
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
}

void ARHIDX12::BeginEvent(const std::string EventName)
{
	PIXBeginEvent(mCommandList.Get(), 0, EventName.c_str());
}

void ARHIDX12::EndEvent()
{
	PIXEndEvent(mCommandList.Get());
}

ABuffer* ARHIDX12::CreateBuffer(std::shared_ptr<ARenderItem> renderItem, const std::string& Name)
{
	return renderItem->mGeo.get();
}
