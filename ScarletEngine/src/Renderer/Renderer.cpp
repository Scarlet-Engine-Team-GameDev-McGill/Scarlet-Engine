#include "Renderer/Renderer.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include "Core/CoreUtils.h"
#include "Renderer/OpenGLRAL/OpenGLRAL.h"

#include "Renderer/Scene.h"
#include "Renderer/Viewport.h"

namespace ScarletEngine
{
	Renderer::Renderer()
		: RAL(nullptr)
	{
	}

	void Renderer::Initialize()
	{

#ifdef RAL_USE_OPENGL
		RAL = std::unique_ptr<IRAL>(new OpenGLRAL);
#elif defined RAL_USE_VULKAN
		// initialize a vulkan RAL
#endif
		// Ensure we have some kind of RAL set up by here
		check(RAL != nullptr);

		RAL->Initialize();
	}


	void Renderer::EndFrame()
	{
		RAL->SwapWindowBuffers();
		RAL->PollWindowEvents();
	}
	
	void Renderer::SetWindowCtx(void* WindowPtr)
	{
		RAL->SetWindowCtx(WindowPtr);
	}

	Viewport* Renderer::CreateViewport(uint32_t Width, uint32_t Height)
	{
		return new Viewport(RAL.get(), Width, Height);
	}

	void Renderer::DrawScene(Scene*, Viewport* ActiveViewport)
	{
		ActiveViewport->Bind();
		// For now default to clearing with Scarlet Red
		RAL->SetClearColorCommand({ 1.0f, 0.13f, 0.f, 1.f });
		RAL->ClearCommand(true, true, true);

		ActiveViewport->Unbind();
	}
}