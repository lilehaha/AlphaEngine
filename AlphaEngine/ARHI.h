#pragma once
#include "ARenderScene.h"
#include "ARenderItem.h"

class ATexture;
class ARHI
{
public:
	ARHI();
	virtual ~ARHI();
	virtual bool Init() = 0;
	virtual void Draw(std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void Update(int CBIndex, std::shared_ptr<ARenderScene> RenderScene, ARenderItem* renderItem) = 0;

	virtual void RenderFrameBegin(std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void CreateCbHeapsAndSrv(const std::string& ActorName, const std::string& MeshName, ARenderItem* RenderItem, std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void ResetCommand(const std::string& PSOName) = 0;
	//virtual void BuildPSO(std::shared_ptr<ARenderItem> RenderItem, AMaterial Mat) = 0;
	virtual void ExecuteCommandLists() = 0;
	virtual void CreateTextureResource(std::shared_ptr<ARenderScene> RenderScene, std::shared_ptr<ATexture> Texture) = 0;
protected:
	static ARHI* RHI;
};

