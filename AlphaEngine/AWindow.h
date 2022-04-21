#pragma once
#include "AGameTimer.h"
#include "AInput.h"

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
	virtual UINT GetWidth();
	virtual UINT GetHeight();
	virtual HWND GetHWND() = 0;
	virtual std::shared_ptr<AInput> GetInput();
protected:
	std::wstring mName;
	UINT mWidth;
	UINT mHeight;
	std::shared_ptr<AInput> mInput;
};