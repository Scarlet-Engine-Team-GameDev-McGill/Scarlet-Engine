#include "Camera.h"

namespace ScarletEngine
{
	Camera::Camera()
		: Position(0, 0, 0)
		, Yaw()
		, Pitch()
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
		const glm::vec3 RightVector = glm::normalize(glm::cross(WorldUp, ForwardVector));
		UpVector = glm::cross(ForwardVector, RightVector);
		RecalculateViewMatrix();
	}

	void Camera::SetRotation(float NewYaw, float NewPitch)
	{
		Yaw = NewYaw;
		Pitch = NewPitch;
		glm::clamp(Yaw, -89.f, 89.f);
		glm::clamp(Pitch, -89.f, 89.f);

		RecalculateBasis();
	}

	void Camera::RecalculateProjection()
	{
		Projection = glm::perspective(glm::radians(FoV), Aspect, NearPlane, FarPlane);
		ViewProjection = Projection * View;
	}

	void Camera::RecalculateBasis()
	{
		glm::vec3 Forward;
		Forward.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
		Forward.y = glm::sin(glm::radians(Pitch));
		Forward.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
		ForwardVector = glm::normalize(Forward);
		RightVector = glm::normalize(glm::cross(ForwardVector, WorldUp));
		UpVector = glm::normalize(glm::cross(RightVector, ForwardVector));

		RecalculateViewMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		View = glm::lookAt(Position, Position + ForwardVector, UpVector);
		ViewProjection = Projection * View;
	}
}