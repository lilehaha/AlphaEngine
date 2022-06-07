#pragma once
#include "ARenderScene.h"
#include "ARenderItem.h"
#include "AResource.h"

class ATexture;
class ARHI
{
public:
	ARHI();
	virtual ~ARHI();
	virtual bool Init() = 0;
	virtual void Draw(std::shared_ptr<ARenderItem> renderItem, const std::string& renderItemName, bool IsDrawDepth, bool isNeedRTV, int RTVNumber, int width, int height) = 0;
	virtual void Update(int CBIndex, std::shared_ptr<ARenderScene> RenderScene, ARenderItem* renderItem) = 0;

	virtual void RenderFrameBegin(std::shared_ptr<ARenderScene> renderResource, const std::string& MatName) = 0;
	virtual void CreateCbHeapsAndSrv(const std::string& ActorName, const std::string& MeshName, ARenderItem* RenderItem, ARenderResource* shadowResource, ARenderResource* HDRResource, std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void ResetCommand(const std::string& PSOName) = 0;
	virtual void RSSetViewports(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth) = 0;
	virtual void RSSetScissorRects(long left, long top, long right, long bottom) = 0;
	virtual void ResourceBarrier(unsigned int NumberBarrier, std::shared_ptr<AResource> Resource, int stateBefore, int stateAfter) = 0;
	virtual void SetPipelineState(std::shared_ptr<ARenderItem> renderItem, AMaterial Mat) = 0;
	virtual void BuildPSO(std::shared_ptr<ARenderItem> renderItem, AMaterial Mat) = 0;
	virtual void ChangePSOState(AMaterial Mat, const APSO& PSO, const std::wstring& Shader) = 0;
	virtual void ClearAndSetRenderTatget(unsigned __int64 ClearRenderTargetHand, unsigned __int64 ClearDepthStencilHand, int numTatgetDescriptors, unsigned __int64 SetRenderTargetHand, bool RTsSingleHandleToDescriptorRange, unsigned __int64 SetDepthStencilHand) = 0;

	virtual void ExecuteCommandLists() = 0;
	virtual void CreateTextureResource(std::shared_ptr<ARenderScene> RenderScene, std::shared_ptr<ATexture> Texture) = 0;

	virtual void BeginEvent(const std::string EventName) = 0;
	virtual void EndEvent() = 0;
	virtual ABuffer* CreateBuffer(std::shared_ptr<ARenderItem> renderItem, const std::string& Name) = 0;
};

