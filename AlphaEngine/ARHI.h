#pragma once
#include "ARenderScene.h"
#include "ARenderItem.h"
class ARHI
{
public:
	ARHI();
	virtual ~ARHI();
	virtual bool Init() = 0;
	virtual void Draw(std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void Update(int CBIndex, ARenderItem* renderItem) = 0;

	virtual void RenderFrameBegin(std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void CreateCbHeapsAndSrv(const std::string& ActorName, const std::string& MeshName, ARenderItem* RenderItem, std::shared_ptr<ARenderScene> RenderScene) = 0;
	virtual void ResetCommand(const std::string& PSOName) = 0;
	virtual void ExecuteCommandLists() = 0;
protected:
	static ARHI* RHI;
};

