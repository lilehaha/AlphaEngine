#include "stdafx.h"
#include "AEngine.h"

using namespace Alpha;
#ifdef PlatformUnknown
int main()
{

}
#elif defined(PlatformWindows)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::shared_ptr<AEngine> AlphaEngine = std::make_shared<AEngine>();
	AlphaEngine->Init();
	while (true)
	{

	}
}
#endif
