#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Camera.h"

namespace ScarletEngine
{
	class RALFramebuffer;
	class IRAL;

	class Viewport
	{
	public:
		Viewport(uint32_t Width, uint32_t Height);
		~Viewport();

		void Bind() const;
		void Unbind() const;

		uint64_t GetColorAttachmentID() const;

		void ResizeFramebuffer(uint32_t NewWidth, uint32_t NewHeight);

		glm::ivec2 GetSize();

		void SetCamera(const SharedPtr<Camera>& InCam) { Cam = InCam; }

		Camera& GetCamera() { return *Cam; }
		const Camera& GetCamera() const { return *Cam; }
	private:
		RALFramebuffer* Framebuffer;
		SharedPtr<Camera> Cam;
	};
}