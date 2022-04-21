#include "stdafx.h"
#include "ACamera.h"

ACamera::ACamera() : mPos({ 1.0f, 1.0f, 1.0f }),
				   mRight({ 0.0f,1.0f,0.0f }),
				   mUp({ 0.0f,0.0f,1.0f }),
				   mForward({ 1.0f,0.0f,0.0f }),
				   mNearZ(0.0f),
				   mFarZ(0.0f),
				   mFovY(0.0f),
				   mNearPlaneHeight(0.0f),
				   mFarPlaneHeight(0.0f),
				   bIsDirty(true)
{

}

ACamera::~ACamera()
{

}

void ACamera::SetACameraPosition(float X, float Y, float Z)
{
	mPos = glm::vec3(X, Y, Z);
}

void ACamera::SetACameraPosition(const glm::vec3& Position)
{
	mPos = Position;
}

glm::vec3 ACamera::GetACameraPosition() const
{
	return mPos;
}

void ACamera::SetRight(const glm::vec3& Right)
{
	mRight = Right;
}

void ACamera::SetUp(const glm::vec3& Up)
{
	mUp = Up;
}

void ACamera::SetForward(const glm::vec3& Forward)
{
	mForward = Forward;
}

glm::vec3 ACamera::GetRight() const
{
	return mRight;
}

glm::vec3 ACamera::GetUp() const
{
	return mUp;
}

glm::vec3 ACamera::GetForward() const
{
	return mForward;
}

void ACamera::SetLens(float FovY, float aspect, float nearZ, float farZ)
{
	mFovY = FovY;
	mAspect = aspect;
	mNearZ = nearZ;
	mFarZ = farZ;

	mNearPlaneHeight = 2.0f * mNearZ * tanf(0.5f * FovY);
	mFarPlaneHeight = 2.0f * mFarZ * tanf(0.5f * FovY);

	mProj = BuildPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
}

glm::mat4x4 ACamera::BuildPerspectiveFovLH(float FovY, float aspect, float nearZ, float farZ)
{
	glm::mat4x4 P;
	P[0][0] = 1 / (aspect * (tanf(FovY / 2)));
	P[0][1] = 0.0f;
	P[0][2] = 0.0f;
	P[0][3] = 0.0f;

	P[1][0] = 0.0f;
	P[1][1] = 1 / (tanf(FovY / 2));
	P[1][2] = 0.0f;
	P[1][3] = 0.0f;

	P[2][0] = 0.0f;
	P[2][1] = 0.0f;
	P[2][2] = farZ / (farZ - nearZ);
	P[2][3] = 1.0f;

	P[3][0] = 0.0f;
	P[3][1] = 0.0f;
	P[3][2] = -nearZ * farZ / (farZ - nearZ);
	P[3][3] = 0.0f;
	return P;
}

void ACamera::LookAt(glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up)
{
	mForward = glm::normalize(Target - Pos);
	mRight = glm::normalize(glm::cross(Up, mForward));
	mUp = glm::cross(mForward, mRight);
	mPos = Pos;
}

glm::mat4x4 ACamera::GetViewMatrix() const
{
	return mView;
}

glm::mat4x4 ACamera::GetProjMatrix() const
{
	return mProj;
}

void ACamera::UpdateViewMatrix()
{
	if (bIsDirty)
	{
		glm::vec3 Right = mRight;
		glm::vec3 Up = mUp;
		glm::vec3 Forward = mForward;
		glm::vec3 Pos = mPos;

		Forward = glm::normalize(Forward);
		Up = glm::normalize(glm::cross(Forward, Right));
		Right = glm::cross(Up, Forward);

		float x = -glm::dot(mPos, mRight);
		float y = -glm::dot(mPos, mUp);
		float z = -glm::dot(mPos, mForward);

		mRight = Right;
		mUp = Up;
		mForward = Forward;

		mView[0][0] = mRight.x;
		mView[1][0] = mRight.y;
		mView[2][0] = mRight.z;
		mView[3][0] = x;

		mView[0][1] = mUp.x;
		mView[1][1] = mUp.y;
		mView[2][1] = mUp.z;
		mView[3][1] = y;

		mView[0][2] = mForward.x;
		mView[1][2] = mForward.y;
		mView[2][2] = mForward.z;
		mView[3][2] = z;

		mView[0][3] = 0.0f;
		mView[1][3] = 0.0f;
		mView[2][3] = 0.0f;
		mView[3][3] = 1.0f;

		bIsDirty = true;
	}
}

void ACamera::SetDirty(bool b)
{
	bIsDirty = b;
}

