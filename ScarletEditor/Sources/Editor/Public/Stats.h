#pragma once

#include "Core.h"
#include "ITickable.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	class StatsPanel : public UIWindow, ITickable
	{
	public:
		StatsPanel();

		virtual void Tick(double DeltaTime) override;
		virtual void DrawWindowContent() override;
	private:
		/** Number of samples for the frame time sliding window mean */
		static const int FrameWindowSize = 50;
		float FrameTimes[FrameWindowSize];

		float FrameTimeSum = 0.f;
		float FrameTimeMean = 0.f;

		uint32_t CurrentFrameTimeIndex = 0;
	};
}