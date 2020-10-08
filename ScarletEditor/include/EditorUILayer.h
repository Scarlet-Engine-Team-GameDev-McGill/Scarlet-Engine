#pragma once

#include "UI/UILayer.h"

namespace ScarletEngine
{
	class EditorUILayer : public UILayer
	{
	public:
		virtual void Initialize() override;
		virtual void Draw() override;
	private:
		void DrawMenuBar();
	};
}