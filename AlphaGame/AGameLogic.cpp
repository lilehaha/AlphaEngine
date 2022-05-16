#include "stdafx.h"
#include "AGameLogic.h"
#include "AEngine.h"

void AGameLogic::Init()
{
	LoadMap("..\\AlphaEngine\\Data\\Map\\Minimal_Default.dat");
}

bool AGameLogic::LoadMap(const std::string& Path)
{
	if (AEngine::GetSingleton().GetScene()->LoadScene(Path)) {
		AEngine::GetSingleton().GetAssetManager()->LoadTextures();
		return true;
	}
	return false;
}

void AGameLogic::Destory()
{

}
