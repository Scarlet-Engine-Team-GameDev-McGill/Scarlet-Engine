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