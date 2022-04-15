#pragma once
#include "stdafx.h"
#include "AGameTimer.h"

class AWindow
{
public:
	static AWindow* InitWindow(UINT Width, UINT Height,const std::wstring& Name);
	static void DestoryWindow(AWindow* Window);

	virtual void CalculateFrameStats(const AGameTimer& gt) = 0;

	virtual bool Run() = 0;
	virtual UINT GetWidth() { return mWidth; }
	virtual UINT GetHeight() { return mHeight; }
	inline HWND GetHWND() { return mHWnd; }
protected:
	AWindow(UINT Width, UINT Height);
	virtual ~AWindow();

protected:
	UINT mWidth;
	UINT mHeight;
	HWND mHWnd;
};

#ifdef PlatformWindows
class AWindowWin32 : public AWindow
{
public:
	AWindowWin32(UINT Width, UINT Height, const std::wstring& Name, void* HWnd);
	virtual ~AWindowWin32();
	virtual bool Run();

	void InstantiateWindow(const std::wstring& Name);
	void CalculateFrameStats(const AGameTimer& gt) override;
private:
	std::wstring mName;
};


#elif defined(PlatformIOS)
class AWindowIOS : public AWindow {};

#elif defined(PlatformAndroid)
class AWindowAndroid : public AWindow {};

#endif

