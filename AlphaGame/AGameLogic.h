#pragma once
class AGameLogic
{
public:
	void Init();
	bool LoadMap(const std::string& Path);
	bool LoadLightData(const std::string& Path);
	void Destory();
};

