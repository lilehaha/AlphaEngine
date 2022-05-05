#include "stdafx.h"
#include "ACameraController.h"
#include "AEngine.h"

ACameraController::ACameraController() : mCamera(nullptr), mSpeed({ 0.1f,0.1f,0.1f })
{
}

ACameraController::~ACameraController()
{

}

void ACameraController::HAxisMove(glm::vec3& Speed)
{
	glm::vec3 f = mCamera->GetForward();
	mCamera->SetACameraPosition(VectorMultiplyAdd(Speed, f, mCamera->GetACameraPosition()));

	mCamera->SetDirty(true);
}

void ACameraController::VAxisMove(glm::vec3& Speed)
{
	glm::vec3 r = mCamera->GetRight();
	mCamera->SetACameraPosition(VectorMultiplyAdd(Speed, r, mCamera->GetACameraPosition()));

	mCamera->SetDirty(true);
}

void ACameraController::LiftMove(glm::vec3& Speed)
{
	glm::vec3 u = mCamera->GetUp();
	mCamera->SetACameraPosition(VectorMultiplyAdd(Speed, u, mCamera->GetACameraPosition()));

	mCamera->SetDirty(true);
}

void ACameraController::Pitch(float Angle)
{
	mCamera->SetUp(glm::rotate(mCamera->GetUp(), Angle,mCamera->GetRight()));
	mCamera->SetForward(glm::rotate(mCamera->GetForward(), Angle,mCamera->GetRight()));

	mCamera->SetDirty(true);
}

void ACameraController::Roll(float Angle)
{
	mCamera->SetRight(glm::rotate(mCamera->GetRight(), Angle, mCamera->GetForward()));
	mCamera->SetUp(glm::rotate(mCamera->GetUp(), Angle, mCamera->GetForward()));

	mCamera->SetDirty(true);
}

void ACameraController::Yaw(float Angle)
{
	mCamera->SetRight(glm::rotate(mCamera->GetRight(), Angle, glm::vec3(0, 0, 1)));
	mCamera->SetForward(glm::rotate(mCamera->GetForward(), Angle, glm::vec3(0, 0, 1)));

	mCamera->SetDirty(true);
}

void ACameraController::Update()
{
	std::pair<float, float> mNewMousePos = AEngine::GetSingleton().GetWindow()->GetInput()->GetMousePosition();
	if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::W))
	{
		HAxisMove(mSpeed);
	}
	else if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::A))
	{
		VAxisMove(-mSpeed);
	}
	else if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::S))
	{
		HAxisMove(-mSpeed);
	}
	else if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::D))
	{
		VAxisMove(mSpeed);
	}else if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::Q))
	{
		LiftMove(mSpeed);
	}
	else if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::E))
	{
		LiftMove(-mSpeed);
	}
	else if (AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyHold(KeyCode::Mouse2))
	{
		float dx = glm::radians(0.05f * static_cast<float>(mNewMousePos.first - mLastMousePos.first));
		float dy = glm::radians(0.05f * static_cast<float>(mNewMousePos.second - mLastMousePos.second));

		Yaw(dx);
		Pitch(dy);
	}
	mLastMousePos = mNewMousePos;
}

void ACameraController::SetCameraObj(std::shared_ptr<ACamera> Camera)
{
	mCamera = Camera;
}


glm::vec3 ACameraController::VectorMultiplyAdd(glm::vec3 MultiplyV1, glm::vec3 MultiplyV2, glm::vec3 addV)
{
	glm::vec3 result;
	result.x = MultiplyV1.x * MultiplyV2.x + addV.x;
	result.y = MultiplyV1.y * MultiplyV2.y + addV.y;
	result.z = MultiplyV1.z * MultiplyV2.z + addV.z;

	return result;
}

