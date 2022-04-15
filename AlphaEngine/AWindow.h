#pragma once
#include "AGameTimer.h"

struct WindowInfo
{
	UINT Width;
	UINT Height;
	std::wstring Name;
};
class AWindow
{
public:
	virtual bool InitWindow(const WindowInfo& mWindowInfo) = 0;
	virtual void CalculateFrameStats(const AGameTimer& gt) = 0;
	virtual bool Run() = 0;
	virtual UINT GetWidth() = 0;
	virtual UINT GetHeight() = 0;
	virtual HWND GetHWND() = 0;
};