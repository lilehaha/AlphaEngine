#include "stdafx.h"
#include "RHIBuilder.h"
#include "ARHIDX12.h"

std::shared_ptr<ARHI> RHIBuilder::CreateRHI()
{
#ifdef PlatformWindows
	return std::make_shared<ARHIDX12>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}
