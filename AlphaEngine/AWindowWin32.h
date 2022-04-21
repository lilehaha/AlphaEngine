#pragma once
#include "AWindow.h"

class AWindowWin32 : public AWindow
{
public:
	AWindowWin32();
	virtual ~AWindowWin32();
	virtual bool Run();
	virtual HWND GetHWND() override;
	bool InitWindow(const WindowInfo& mWindowInfo) override;
	void CalculateFrameStats(const AGameTimer& gt) override;
private:
	HWND mHWnd;
};
