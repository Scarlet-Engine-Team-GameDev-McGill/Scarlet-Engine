#pragma once

#include "Core/Core.h"
#include "UI/UIWidget.h"

namespace ScarletEngine
{
	class StatsPanel : public UIWidget, ITickable
	{
	public:
		virtual void Tick(double DeltaTime) override;
		virtual void Draw() override;
	private:
		static const int FrameWindowSize = 50;
		float FrameTimes[FrameWindowSize];

		float FrameTimeSum = 0.f;
		float FrameTimeMean = 0.f;

		uint32_t CurrentFrameTimeIndex = 0;
	};
}