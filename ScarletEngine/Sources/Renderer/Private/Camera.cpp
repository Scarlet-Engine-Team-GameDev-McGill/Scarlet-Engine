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
		RecalculateProjection(/* bUpdateViewProj = */ false);
		RecalculateViewMatrix();
	}

	void Camera::ExtractEulerFromBasis()
	{
		// Pitch = 90 - (angle between forward and X-Z plane). UpVector is the normal vector of the X-Z plane
		// can be simplified here since we have two unit vectors
		const float Dot = glm::dot(ForwardVector, WorldUp);
		Pitch = glm::degrees(glm::radians(90.f) - glm::acos(Dot));

		// project the forward vector onto the horizontal plane by setting y = 0 and normalizing
		const glm::vec3 ProjectedForward = glm::normalize(glm::vec3(ForwardVector.x, 0.f, ForwardVector.z));
		Yaw = glm::degrees(glm::acos(glm::dot(ProjectedForward, WorldRight)));
		// If dot here is positive, our angle will be greater than 180 (acos returns 0-180) so we need to take the inverse
		if (glm::dot(ProjectedForward, WorldForward))
		{
			Yaw = 360.f - Yaw;
		}
	}

	void Camera::LookAtPoint(const glm::vec3 Point)
	{
		ForwardVector = glm::normalize(Point - Position);
		RightVector = glm::normalize(glm::cross(WorldUp, ForwardVector));
		UpVector = glm::cross(ForwardVector, RightVector);

		ExtractEulerFromBasis();
		RecalculateViewMatrix();
	}

	void Camera::SetRotation(float NewYaw, float NewPitch)
	{
		Yaw = NewYaw;
		Pitch = glm::clamp(NewPitch, -89.f, 89.f);

		RecalculateBasis();
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

	void Camera::RecalculateProjection(bool bUpdateViewProj)
	{
		Projection = glm::perspective(glm::radians(FoV), Aspect, NearPlane, FarPlane);
		if (bUpdateViewProj)
		{
			RecalculateViewProj();
		}
	}

	void Camera::RecalculateViewMatrix(bool bUpdateViewProj)
	{
		View = glm::lookAt(Position, Position + ForwardVector, UpVector);
		if (bUpdateViewProj) 
		{
			RecalculateViewProj();
		}
	}
}
