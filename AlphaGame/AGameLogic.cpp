#include "stdafx.h"
#include "AGameLogic.h"
#include "AEngine.h"

void AGameLogic::Init()
{
	LoadMap("..\\AlphaEngine\\Data\\Map\\Minimal_Default.dat");
	LoadLightData("..\\AlphaEngine\\Data\\Light\\LightSource.dat");
}

bool AGameLogic::LoadMap(const std::string& Path)
{
	if (AEngine::GetSingleton().GetScene()->LoadScene(Path)) {
		AEngine::GetSingleton().GetAssetManager()->LoadTextures();
		return true;
	}
	return false;
}

bool AGameLogic::LoadLightData(const std::string& Path)
{
	if (AEngine::GetSingleton().GetScene()->LoadLight(Path)) {
		return true;
	}
	return false;
}

void AGameLogic::Destory()
{

}
