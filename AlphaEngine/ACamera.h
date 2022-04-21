#pragma once
#include "stdafx.h"

class ACamera
{
public:
	ACamera();
	virtual ~ACamera();

	void SetACameraPosition(float X, float Y, float Z);
	void SetACameraPosition(const glm::vec3& Position);
	glm::vec3 GetACameraPosition() const;

	void SetRight(const glm::vec3& Right);
	void SetUp(const glm::vec3& Up);
	void SetForward(const glm::vec3& Forward);
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetForward() const;

	void SetLens(float FovY, float aspect, float nearZ, float farZ);
	glm::mat4x4 BuildPerspectiveFovLH(float FovY, float aspect, float nearZ, float farZ);

	void LookAt(glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up);

	glm::mat4x4 GetViewMatrix() const;
	glm::mat4x4 GetProjMatrix() const;

	void UpdateViewMatrix();
	void SetDirty(bool b);

private:
	glm::vec3 mPos;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mForward;

	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;

	float mNearPlaneHeight;
	float mFarPlaneHeight;

	glm::mat4x4 mView = glm::identity<glm::mat4x4>();
	glm::mat4x4 mProj = glm::identity<glm::mat4x4>();

	bool bIsDirty;
};

