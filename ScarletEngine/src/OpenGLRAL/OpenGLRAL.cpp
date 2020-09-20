#include "OpenGLRAL/OpenGLRAL.h"

#include "Core/Core.h"
#include "OpenGLRAL/OpenGLResources.h"
#include "AssetManager/AssetManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ScarletEngine
{
	void FramebufferResizeCallback(GLFWwindow*, int Width, int Height)
	{
		glViewport(0, 0, Width, Height);
	}

	void WindowCloseCallback(GLFWwindow*)
	{
		Engine::Get().SignalQuit();
	}

	void OpenGLRAL::Initialize()
	{
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
		glfwSwapInterval(1);

		check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(Window, FramebufferResizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void OpenGLRAL::SetWindowCtx(void* WindowPtr)
	{
		glfwMakeContextCurrent((GLFWwindow*)WindowPtr);
	}

	void OpenGLRAL::Terminate()
	{
		glfwTerminate();
	}

	void OpenGLRAL::SwapWindowBuffers() const
	{
		glfwSwapBuffers(Window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRAL::PollWindowEvents() const
	{
		glfwPollEvents();
	}

	void OpenGLRAL::SetClearColorCommand(const glm::vec4& ClearColor) const
	{
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	}

	void OpenGLRAL::ClearCommand(bool bColor, bool bDepth, bool bStencil) const
	{
		GLbitfield ClearField = 0;
		if (bColor) ClearField |= GL_COLOR_BUFFER_BIT;
		if (bDepth) ClearField |= GL_DEPTH_BUFFER_BIT;
		if (bStencil) ClearField |= GL_STENCIL_BUFFER_BIT;

		glClear(ClearField);
	}

	RALFramebuffer* OpenGLRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const
	{
		return GlobalAllocator<OpenGLFramebuffer>::New(Width, Height, Samples);
	}

	ScarletEngine::RALTexture2D* OpenGLRAL::CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const
	{
		return GlobalAllocator<OpenGLTexture2D>::New(AssetHandle);
	}

	RALVertexBuffer* OpenGLRAL::CreateVertexBuffer(uint32_t Size, uint32_t Usage) const
	{
		return GlobalAllocator<OpenGLVertexBuffer>::New(Size, Usage);
	}

	RALShader* OpenGLRAL::CreateShader(RALShaderStage Stage, const Array<uint8_t>& ShaderCode) const
	{
		return GlobalAllocator<OpenGLShader>::New(Stage, ShaderCode);
	}

	RALShaderProgram* OpenGLRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const
	{
		return GlobalAllocator<OpenGLShaderProgram>::New(InVertexShader, InPixelShader, InGeometryShader, InComputeShader);
	}
}