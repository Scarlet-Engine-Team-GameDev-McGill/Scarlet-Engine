#pragma once

#include <memory>
#include "RAL/RAL.h"

namespace ScarletEngine
{
	class Renderer
	{
	public:
		void Initialize();

		void DrawFrame();

		static Renderer& Get() { static Renderer Instance; return Instance; }
	private:
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;

		std::unique_ptr<IRAL> RAL;
	};
}