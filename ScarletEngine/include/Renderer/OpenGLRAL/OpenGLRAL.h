#pragma once

#include "Renderer/RAL/RAL.h"

struct GLFWwindow;

namespace ScarletEngine
{
	class OpenGLRAL : public IRAL
	{
	public:
		virtual void Initialize() override;
		virtual void Terminate() override;

		// #todo: move window functions out of RAL
		virtual void SwapWindowBuffers() const override;
		virtual void PollWindowEvents() const override;

		virtual const char* GetBackendName() const { return "OpenGL"; }

		virtual RALVertexBuffer* CreateVertexBuffer(uint32_t Size, uint32_t Usage) const override;
		virtual RALShader* CreateShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode) const override;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const override;
	private:
		// #todo: this shouldn't really be part of the RAL but rather some other windowing abstraction layer
		GLFWwindow* Window = nullptr;
	};
}