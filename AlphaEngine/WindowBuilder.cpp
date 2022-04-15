#include "stdafx.h"
#include "WindowBuilder.h"
#include "AWindowWin32.h"

std::shared_ptr<AWindow> WindowBuilder::CreateMainWindow()
{
#ifdef PlatformWindows
	return std::make_shared<AWindowWin32>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}
