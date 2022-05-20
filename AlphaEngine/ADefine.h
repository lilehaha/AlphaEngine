#pragma once

#ifndef PlatformWindows
#define PlatformWindows
#endif 
#ifndef DXRender
#define DXRender
#endif 

#define ReleasePtr(Ptr) \
if (Ptr != nullptr)\
{\
	delete(Ptr);\
	Ptr = nullptr;\
}