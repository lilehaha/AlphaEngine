#pragma once
#include "stdafx.h"
#include "ACamera.h"

class ACameraController
{
public:
	ACameraController();
	~ACameraController();

	
	void HAxisMove(glm::vec3& Speed);
	void VAxisMove(glm::vec3& Speed);
	void LiftMove(glm::vec3& Speed);

	void Pitch(float Angle);
	void Roll(float Angle);
	void Yaw(float Angle);


	void Update();
	void SetCameraObj(std::shared_ptr<ACamera> Camera);
	
	glm::vec3 VectorMultiplyAdd(glm::vec3 MultiplyV1, glm::vec3 MultiplyV2, glm::vec3 addV);
private:
	std::shared_ptr<ACamera> mCamera;
	glm::vec3 mSpeed;
	std::pair<float, float> mLastMousePos;
	bool bIsMove;
};

