#include "Panels/Stats.h"

#include "Core/Memory/MemoryTracker.h"

namespace ScarletEngine
{
	StatsPanel::StatsPanel()
		: UIWidget("Stats")
		, FrameTimes()
	{

	}

	void StatsPanel::Tick(double DeltaTime)
	{
		ZoneScoped
		FrameTimeSum -= FrameTimes[CurrentFrameTimeIndex];
		FrameTimes[CurrentFrameTimeIndex] = (float)DeltaTime;
		FrameTimeSum += (float)DeltaTime;

		CurrentFrameTimeIndex = (CurrentFrameTimeIndex + 1) % FrameWindowSize;

		FrameTimeMean = (float)(FrameTimeSum / FrameWindowSize);
	}

	void StatsPanel::Draw()
	{
		ZoneScoped
		ImGui::Text("CPU");
		ImGui::Separator();

		const ImVec2 ContentRegion = ImGui::GetContentRegionAvail();
		char Buff[32];
		snprintf(Buff, 32, "Frame time: %.2f ms", FrameTimeMean);
		{
			ZoneScopedN("Plot Frametimes")
			ImGui::PlotLines("", FrameTimes, IM_ARRAYSIZE(FrameTimes), CurrentFrameTimeIndex, Buff, 0.f, 20.f, ImVec2(ContentRegion.x, 80.0f));
		}
		ImGui::Text("FPS: %.1f", (double)(1.f / FrameTimeMean) * 1000.f);

		ImGui::Separator();
		ImGui::Text("Memory");
		ImGui::Text("Number of allocations: %lu", MemoryTracker::Get().GetNumAllocs());
		ImGui::Text("Memory used: %s", StringUtils::CreateBytesString(MemoryTracker::Get().GetMemUsed()).c_str());

		ImGui::Text("GPU");
		ImGui::Separator();
		// No gpu stats available yet
	}
}