#pragma once

#include "Core/CoreMinimal.h"

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
	private:
		RALFramebuffer* Framebuffer;
	};
}