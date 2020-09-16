#pragma once

#include "Core/CoreUtils.h"

namespace ScarletEngine
{
	class RALFramebuffer;
	class IRAL;

	class Viewport
	{
	public:
		Viewport(IRAL* RAL, uint32_t Width, uint32_t Height);

		void Bind() const;
		void Unbind() const;

		uint64_t GetColorAttachmentID() const;

		void ResizeFramebuffer(uint32_t NewWidth, uint32_t NewHeight);

		glm::ivec2 GetSize();
	private:
		RALFramebuffer* Framebuffer;
	};
}