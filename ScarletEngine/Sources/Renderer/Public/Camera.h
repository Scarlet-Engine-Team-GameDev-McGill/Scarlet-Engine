#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	class Camera
	{
	public:
		Camera();

		inline void SetAspectRatio(float InAspect) { Aspect = InAspect; RecalculateProjection(); }
		inline void SetFoV(float InFoV) { FoV = InFoV; RecalculateProjection(); }
		inline void SetClippingPlanes(float InNearPlane, float InFarPlane) { NearPlane = InNearPlane; FarPlane = InFarPlane; RecalculateProjection(); }
		inline void SetBasis(glm::vec3 InUp, glm::vec3 InForward) { UpVector = InUp; ForwardVector = InForward; RecalculateViewMatrix(); };
		void LookAtPoint(const glm::vec3 Point);

		inline void Translate(const glm::vec3& Translation) { Position += Translation; RecalculateViewMatrix(); }
		inline void Rotate(glm::vec2 DeltaAngles) { Rotate(DeltaAngles.x, DeltaAngles.y); }
		inline void Rotate(float DeltaYaw, float DeltaPitch) { SetRotation(Yaw + DeltaYaw, Pitch + DeltaPitch); }
		void SetRotation(float NewYaw, float NewPitch);
		inline void SetPosition(const glm::vec3& NewPos) { Position = NewPos; RecalculateViewMatrix(); }

		inline const glm::mat4& GetProj() const { return Projection; }
		inline const glm::mat4& GetView() const { return View; }
		inline const glm::mat4& GetViewProj() const { return ViewProjection; }
		inline const glm::vec3& GetPosition() const { return Position; }
		inline glm::vec3 GetUpVector() const { return UpVector; }
		inline glm::vec3 GetForwardVector() const { return ForwardVector; }
		inline glm::vec3 GetRightVector() const { return RightVector; }
		inline float GetSensitivity() const { return Sensitivity; }
		inline float GetSpeed() const { return Speed; }
		
		void SetView(const glm::mat4& InView) { View = InView; RecalculateViewProj(); }
	private:
		void RecalculateProjection();
		void RecalculateBasis();
		void RecalculateViewMatrix();
		inline void RecalculateViewProj() { ViewProjection = Projection * View; }
	private:
		glm::mat4 Projection;
		glm::mat4 View;
		glm::mat4 ViewProjection;

		/* View fields */
		glm::vec3 Position;
		float Yaw;
		float Pitch;
		glm::vec3 UpVector;
		glm::vec3 ForwardVector;
		glm::vec3 RightVector;

		/* Projection fields */
		float NearPlane;
		float FarPlane;
		float FoV;
		float Aspect;

		/* Movement */
		float Sensitivity = 50.f;
		float Speed = 10.f;
	};
}