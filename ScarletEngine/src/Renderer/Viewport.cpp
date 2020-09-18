#include "Renderer/Viewport.h"
#include "Renderer/RAL/RAL.h"

namespace ScarletEngine
{
	Viewport::Viewport(IRAL* RAL, uint32_t Width, uint32_t Height)
	{
		check(RAL);
		Framebuffer = RAL->CreateFramebuffer(Width, Height, 1);
	}

	Viewport::~Viewport()
	{
		delete Framebuffer;
	}

	void Viewport::Bind() const
	{
		Framebuffer->Bind();
	}

	void Viewport::Unbind() const
	{
		Framebuffer->Unbind();
	}

	uint64_t Viewport::GetColorAttachmentID() const
	{
		return Framebuffer->GetColorAttachmentID();
	}

	glm::ivec2 Viewport::GetSize()
	{
		return Framebuffer->GetSize();
	}

	void Viewport::ResizeFramebuffer(uint32_t NewWidth, uint32_t NewHeight)
	{
		Framebuffer->Resize(NewWidth, NewHeight);
	}
}