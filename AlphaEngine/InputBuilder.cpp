#include "stdafx.h"
#include "InputBuilder.h"
#include "AInputWin32.h"

std::shared_ptr<AInput> InputBuilder::CreateInput()
{
#ifdef PlatformWindows
	return std::make_shared<AInputWin32>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}
