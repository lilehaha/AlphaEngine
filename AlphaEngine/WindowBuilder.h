#pragma once
#include "AWindow.h"

class WindowBuilder
{
public:
	std::shared_ptr<AWindow> CreateMainWindow();
};

