#include "RenderModule.h"
#include "CoreUtils.h"
#include "OpenGLRAL.h"
#include "Transform.h"
#include "SceneProxy.h"
#include "Viewport.h"
#include "SystemScheduler.h"
#include "RenderSystem.h"

namespace ScarletEngine
{
	void RenderModule::Startup()
	{
		ZoneScoped
#ifdef RAL_USE_OPENGL
		RAL::Instance = UniquePtr<RAL>(ScarNew(OpenGLRAL));
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

		SystemScheduler::Get().RegisterSystem<SMCRenderSystem>();
	}

	void RenderModule::Shutdown()
	{
		RAL::Get().Terminate();
		RAL::Instance.reset();
		RAL::API = RenderAPI::Invalid;
	}

	void RenderModule::PostUpdate()
	{
		ZoneScoped
	}
	
	Viewport* RenderModule::CreateViewport(uint32_t Width, uint32_t Height)
	{
		ZoneScoped
		return ScarNew(Viewport, Width, Height);
	}

	void RenderModule::DrawScene(SceneProxy* Scene, Viewport* ActiveViewport)
	{
		ZoneScoped
		if (Scene && ActiveViewport)
		{
			ActiveViewport->Bind();
			RAL::Get().SetClearColorCommand({ 0.1f, 0.1f, 0.1f, 1.f });
			RAL::Get().ClearCommand(true, true, true);

			ActiveViewport->Bind();
			for (const auto& [Trans, SMC] : Scene->SMCs)
			{
				RALShaderProgram* Shader = SMC->Shader;
				Shader->Bind();
				Shader->SetUniformMat4(Trans->GetTransformMatrix() , "model");
				Shader->SetUniformMat4(ActiveViewport->GetCamera().GetViewProj(), "vp");
				Shader->SetUniformVec3(ActiveViewport->GetCamera().GetPosition(), "CameraPos");
				RAL::Get().DrawVertexArray(SMC->VertexArray);
				Shader->Unbind();
			}
		
			ActiveViewport->Unbind();
		}
		
	}
}