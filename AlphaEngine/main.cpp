#include "stdafx.h"
#include "AEngine.h"

using namespace Alpha;
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
	std::unique_ptr<AEngine> AlphaEngine = std::make_unique<AEngine>();
	if (AlphaEngine->Init())
	{
		AlphaEngine->Start();
	}
	else
	{
		MessageBox(0, L"Engine Init Failed !", 0, 0);
	}
}
#endif
