#include "Renderer/Viewport.h"
#include "RAL/RAL.h"

namespace ScarletEngine
{
	Viewport::Viewport(uint32_t Width, uint32_t Height)
	{
		Framebuffer = RAL::Get().CreateFramebuffer(Width, Height, 4);
	}

	Viewport::~Viewport()
	{
		GlobalAllocator<RALFramebuffer>::Free(Framebuffer);
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
		ZoneScoped
		Framebuffer->Resize(NewWidth, NewHeight);
	}
}