#pragma once

#include "UI/UILayer.h"

namespace ScarletEngine
{
	class ImGuiUILayer : public UILayer
	{
	public:
		virtual void Initialize() override;
		virtual void BeginFrame() override;
		virtual void Draw() override;
		virtual void Terminate() override;
	private:
	};
}