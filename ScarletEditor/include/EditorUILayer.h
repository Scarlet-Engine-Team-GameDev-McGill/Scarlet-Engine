#pragma once

#include "UI/ImGuiUILayer.h"

namespace ScarletEngine
{
	class EditorUILayer : public ImGuiUILayer
	{
	public:
		virtual void Initialize() override;
		virtual void BeginFrame() override;
		virtual void Draw() override;
		virtual void Terminate() override;
	private:
		void DrawMenuBar();
	};
}