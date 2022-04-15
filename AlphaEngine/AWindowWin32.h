#pragma once
#include "AWindow.h"

class AWindowWin32 : public AWindow
{
public:
	AWindowWin32();
	~AWindowWin32();
	virtual bool Run();

	UINT GetWidth() override;
	UINT GetHeight() override;
	HWND GetHWND() override;
	bool InitWindow(const WindowInfo& mWindowInfo) override;
	void CalculateFrameStats(const AGameTimer& gt) override;
private:
	std::wstring mName;
	UINT mWidth;
	UINT mHeight;
	HWND mHWnd;
};
