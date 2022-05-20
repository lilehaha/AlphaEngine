#pragma once
#include "stdafx.h"
#include "ARenderResource.h"
class ADirectionalLight : public ARenderResource
{
public:
	glm::vec4 Location;
	glm::vec3 Direction;
	float Brightness;
};

