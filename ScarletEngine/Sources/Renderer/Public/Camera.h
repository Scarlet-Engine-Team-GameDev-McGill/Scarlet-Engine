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
		void LookAtPoint(const glm::vec3 Point);

		inline void Translate(const glm::vec3& Translation) { Position += Translation; RecalculateViewMatrix(); }
		inline void SetPosition(const glm::vec3& NewPos) { Position = NewPos; RecalculateViewMatrix(); }

		inline const glm::mat4& GetProj() const { return Projection; }
		inline const glm::mat4& GetView() const { return View; }
		inline const glm::mat4& GetViewProj() const { return ViewProjection; }
		inline const glm::vec3& GetPosition() const { return Position; }

		void SetView(const glm::mat4& InView) { View = InView; RecalculateViewProj(); }
	private:
		void RecalculateProjection();
		void RecalculateViewMatrix();
		void RecalculateViewProj() { ViewProjection = Projection * View; }
	private:
		glm::mat4 Projection;
		glm::mat4 View;
		glm::mat4 ViewProjection;

		glm::vec3 Position;

		glm::vec3 UpVector;
		glm::vec3 ForwardVector;

		float NearPlane;
		float FarPlane;
		float FoV;
		float Aspect;
	};
}