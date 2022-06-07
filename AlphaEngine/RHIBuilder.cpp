#include "stdafx.h"
#include "RHIBuilder.h"
#include "ARHIDX12.h"
#include "AShadowResource.h"
#include "DXRHIResource.h"
#include "AHDRResource.h"

std::shared_ptr<ARHI> RHIBuilder::CreateRHI()
{
#ifdef PlatformWindows
	return std::make_shared<ARHIDX12>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}

std::shared_ptr<ARHIResource> RHIBuilder::CreateRHIResource()
{
#ifdef PlatformWindows
	return std::make_shared<DXRHIResource>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}

std::shared_ptr<ARenderResource> RHIBuilder::CreateShadowResource()
{
#ifdef PlatformWindows
	return std::make_shared<DXShadowResource>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}

std::shared_ptr<ARenderResource> RHIBuilder::CreateHDRResource()
{
#ifdef PlatformWindows
	return std::make_shared<DXHDRResource>();
#elif defined(PlatformIOS)

#elif defined(PlatformAndroid)

#endif
}
