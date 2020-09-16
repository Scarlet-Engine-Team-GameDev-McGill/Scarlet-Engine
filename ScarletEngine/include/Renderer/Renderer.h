#pragma once

#include <memory>
#include <vector>
#include "RAL/RAL.h"

namespace ScarletEngine
{
	class Scene;
	class Viewport;

	class Renderer
	{
	public:
		void Initialize();

		void EndFrame();

		void* GetWindowPtr() { return RAL->GetWindowPtr(); }
		void SetWindowCtx(void* WindowPtr);

		Viewport* CreateViewport(uint32_t Width, uint32_t Height);
		void DrawScene(Scene* ActiveScene, Viewport* ActiveViewport);

		static Renderer& Get() { static Renderer Instance; return Instance; }
	private:
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
	private:
		std::unique_ptr<IRAL> RAL;
	};
}