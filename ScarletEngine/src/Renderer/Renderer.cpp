#include "Renderer/Renderer.h"

#include "Core/CoreUtils.h"
#include "OpenGLRAL/OpenGLRAL.h"

#include "Core/Transform.h"
#include "Renderer/SceneProxy.h"
#include "Renderer/Viewport.h"

namespace ScarletEngine
{
	Renderer::Renderer()
	{
		ZoneScoped
	}

	void Renderer::Initialize()
	{
		ZoneScoped
#ifdef RAL_USE_OPENGL
		RAL::Instance = UniquePtr<RAL>(GlobalAllocator<OpenGLRAL>::New());
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
		ZoneScoped
		RAL::Get().SwapWindowBuffers();
		RAL::Get().PollWindowEvents();
	}
	
	void Renderer::SetWindowCtx(void* WindowPtr)
	{
		ZoneScoped
		RAL::Get().SetWindowCtx(WindowPtr);
	}

	Viewport* Renderer::CreateViewport(uint32_t Width, uint32_t Height)
	{
		ZoneScoped
		return GlobalAllocator<Viewport>::New(Width, Height);
	}

	void Renderer::DrawScene(SceneProxy* Scene, Viewport* ActiveViewport)
	{
		ZoneScoped
		ActiveViewport->Bind();
		RAL::Get().SetClearColorCommand({ 0.1f, 0.1f, 0.1f, 1.f });
		RAL::Get().ClearCommand(true, true, true);

		ActiveViewport->Bind();
		for (auto Pair : Scene->SMCs)
		{
			RALShaderProgram* Shader = Pair.second->Shader;
			Shader->Bind();
			Shader->SetUniformMat4(Pair.first->GetTransformMatrix() , "model");
			Shader->SetUniformMat4(ActiveViewport->GetCamera().GetViewProj(), "vp");
			Shader->SetUniformVec3(ActiveViewport->GetCamera().GetPosition(), "CameraPos");
			RAL::Get().DrawVertexArray(Pair.second->VertexArray);
			Shader->Unbind();
		}
		
		
		ActiveViewport->Unbind();
	}
}