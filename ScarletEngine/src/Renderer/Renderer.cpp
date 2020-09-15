#include "Renderer/Renderer.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include "Core/CoreUtils.h"
#include "Renderer/OpenGLRAL/OpenGLRAL.h"

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


	void Renderer::DrawFrame()
	{
		RAL->SwapWindowBuffers();
		RAL->PollWindowEvents();
	}
}