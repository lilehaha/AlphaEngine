#pragma once
#include "stdafx.h"

class ARHI
{
public: 
	ARHI();
	virtual ~ARHI();
	virtual bool Init() = 0;
	virtual void Draw() = 0;
protected:
	static ARHI* RHI;
};

