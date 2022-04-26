#include "stdafx.h"
#include "AGameInstance.h"

#ifdef PlatformUnknown
int main()
{
	std::cout << "PlatformUnknown" << std::endl;
	return 0;
}
#elif defined(PlatformWindows)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	try
	{
		std::unique_ptr<AGameInstance> mGameInstance = std::make_unique<AGameInstance>();
		mGameInstance->Init(hInstance);
		mGameInstance->UpDate();
		mGameInstance->Destroy();
		return 0;
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}
#endif
