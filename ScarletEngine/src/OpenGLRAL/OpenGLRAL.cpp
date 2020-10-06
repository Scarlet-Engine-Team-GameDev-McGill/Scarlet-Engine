#include "OpenGLRAL/OpenGLRAL.h"

#include "Core/Core.h"
#include "OpenGLRAL/OpenGLResources.h"
#include "AssetManager/AssetManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <TracyOpenGL.hpp>

namespace ScarletEngine
{
	void FramebufferResizeCallback(GLFWwindow*, int Width, int Height)
	{
		ZoneScoped
		glViewport(0, 0, Width, Height);
	}

	void WindowCloseCallback(GLFWwindow*)
	{
		ZoneScoped
		GEngine->SignalQuit();
	}

	void OpenGLRAL::Initialize()
	{
		ZoneScoped
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// #todo: remove hard coded window title
		Window = glfwCreateWindow(800, 600, "Scarlet Editor", nullptr, nullptr);
		if (Window == nullptr)
		{
			glfwTerminate();
			check(false);
		}

		// Set the window icon;
		SharedPtr<TextureHandle> LogoTex = AssetManager::LoadTextureFile("../ScarletEngine/content/scarlet_logo.png");
		GLFWimage Image;
		Image.pixels = LogoTex->PixelDataBuffer;
		Image.width = LogoTex->Width;
		Image.height = LogoTex->Height;

		glfwSetWindowIcon(Window, 1, &Image);

		glfwMakeContextCurrent(Window);
		glfwSwapInterval(0);

		check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(Window, FramebufferResizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void OpenGLRAL::SetWindowCtx(void* WindowPtr)
	{
		ZoneScoped
		glfwMakeContextCurrent((GLFWwindow*)WindowPtr);
	}

	void OpenGLRAL::Terminate()
	{
		ZoneScoped
		glfwTerminate();
	}

	void OpenGLRAL::SwapWindowBuffers() const
	{
		ZoneScoped
		glfwSwapBuffers(Window);
		//TracyGpuCollect
	}

	void OpenGLRAL::PollWindowEvents() const
	{
		ZoneScoped
		glfwPollEvents();
	}

	void OpenGLRAL::SetClearColorCommand(const glm::vec4& ClearColor) const
	{
		ZoneScoped
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	}

	void OpenGLRAL::ClearCommand(bool bColor, bool bDepth, bool bStencil) const
	{
		ZoneScoped
		GLbitfield ClearField = 0;
		if (bColor) ClearField |= GL_COLOR_BUFFER_BIT;
		if (bDepth) ClearField |= GL_DEPTH_BUFFER_BIT;
		if (bStencil) ClearField |= GL_STENCIL_BUFFER_BIT;

		{
			//TracyGpuZone("Clear");
			glClear(ClearField);
		}
	}

	RALFramebuffer* OpenGLRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLFramebuffer>::New(Width, Height, Samples);
	}

	ScarletEngine::RALTexture2D* OpenGLRAL::CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLTexture2D>::New(AssetHandle);
	}

	RALVertexBuffer* OpenGLRAL::CreateVertexBuffer(uint32_t Size, uint32_t Usage) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLVertexBuffer>::New(Size, Usage);
	}

	RALShader* OpenGLRAL::CreateShader(RALShaderStage Stage, const Array<uint8_t>& ShaderCode) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLShader>::New(Stage, ShaderCode);
	}

	RALShaderProgram* OpenGLRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLShaderProgram>::New(InVertexShader, InPixelShader, InGeometryShader, InComputeShader);
	}
}