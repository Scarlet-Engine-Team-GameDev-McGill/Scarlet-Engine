#include "Renderer/Renderer.h"

#include "Core/CoreUtils.h"
#include "OpenGLRAL/OpenGLRAL.h"

#include "Renderer/Scene.h"
#include "Renderer/Viewport.h"

namespace ScarletEngine
{
	Renderer::Renderer()
	{
	}

	void Renderer::Initialize()
	{

#ifdef RAL_USE_OPENGL
		RAL::Instance = UniquePtr<RAL>(new OpenGLRAL);
		RAL::API = RenderAPI::OpenGL;
#elif defined RAL_USE_VULKAN
		// initialize a vulkan RAL

#else
		RAL::API = RenderAPI::Invalid;
#endif
		// Ensure we have some kind of RAL set up by here
		check(RAL::Instance != nullptr);
		check(RAL::API != RenderAPI::Invalid);

		RAL::Get().Initialize();
	}


	void Renderer::EndFrame()
	{
		RAL::Get().SwapWindowBuffers();
		RAL::Get().PollWindowEvents();
	}
	
	void Renderer::SetWindowCtx(void* WindowPtr)
	{
		RAL::Get().SetWindowCtx(WindowPtr);
	}

	Viewport* Renderer::CreateViewport(uint32_t Width, uint32_t Height)
	{
		return GlobalAllocator::Alloc<Viewport>(Width, Height);
	}

	void Renderer::DrawScene(Scene*, Viewport* ActiveViewport)
	{
		ActiveViewport->Bind();
		// For now default to clearing with Scarlet Red
		RAL::Get().SetClearColorCommand({ 1.0f, 0.13f, 0.f, 1.f });
		RAL::Get().ClearCommand(true, true, true);

		ActiveViewport->Unbind();
	}
}