#pragma once

#ifndef PlatformWindows
#define PlatformWindows
#endif 

#define ReleasePtr(Ptr) \
if (Ptr != nullptr)\
{\
	delete(Ptr);\
	Ptr = nullptr;\
}