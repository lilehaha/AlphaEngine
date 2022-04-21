#include "stdafx.h"
#include "AWindow.h"
#include "InputBuilder.h"

UINT AWindow::GetWidth()
{
	return mWidth;
}

UINT AWindow::GetHeight()
{
	return mHeight;
}

std::shared_ptr<AInput> AWindow::GetInput()
{
	return mInput;
}
