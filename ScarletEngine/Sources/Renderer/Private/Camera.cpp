#include "Camera.h"

namespace ScarletEngine
{
	Camera::Camera()
		: Position(0, 0, 0)
		, UpVector(0.0, 0.0, 1.0)
		, ForwardVector(0.0, 1.0, 0.0)
		, NearPlane(0.1f)
		, FarPlane(100.f)
		, FoV(45.f)
		, Aspect(0.0)
	{
		RecalculateProjection();
		RecalculateViewMatrix();
	}

	void Camera::LookAtPoint(const glm::vec3 Point)
	{
		ForwardVector = glm::normalize(Point - Position);
		glm::vec3 GlobalUp = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 RightVector = glm::normalize(glm::cross(GlobalUp, ForwardVector));
		UpVector = glm::cross(ForwardVector, RightVector);
		RecalculateViewMatrix();
	}

	void Camera::RecalculateProjection()
	{
		Projection = glm::perspective(glm::radians(FoV), Aspect, NearPlane, FarPlane);
		ViewProjection = Projection * View;
	}

	void Camera::RecalculateViewMatrix()
	{
		View = glm::lookAt(Position, Position + ForwardVector, UpVector);
		ViewProjection = Projection * View;
	}
}