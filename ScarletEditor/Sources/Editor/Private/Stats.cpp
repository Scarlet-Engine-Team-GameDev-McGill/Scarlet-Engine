#include "Stats.h"
#include "RAL.h"
#include "Memory/MemoryTracker.h"

namespace ScarletEngine
{
	StatsPanel::StatsPanel()
		: UIWindow("Stats")
		, FrameTimes()
	{

	}

	void StatsPanel::Tick(double DeltaTime)
	{
		FrameTimeSum -= FrameTimes[CurrentFrameTimeIndex];
		FrameTimes[CurrentFrameTimeIndex] = static_cast<float>(DeltaTime);
		FrameTimeSum += static_cast<float>(DeltaTime);

		CurrentFrameTimeIndex = (CurrentFrameTimeIndex + 1) % FrameWindowSize;

		FrameTimeMean = (FrameTimeSum / static_cast<float>(FrameWindowSize));
	}

	void StatsPanel::DrawWindowContent()
	{
		ImGui::Text("CPU");
		ImGui::Separator();

		const ImVec2 ContentRegion = ImGui::GetContentRegionAvail();
		char Buff[32];
		snprintf(Buff, 32, "Frame time: %.2f ms", FrameTimeMean);
		{
			ZoneScopedN("Plot Frametimes")
			ImGui::PlotLines("", FrameTimes, IM_ARRAYSIZE(FrameTimes), CurrentFrameTimeIndex, Buff, 0.0f, 0.02f, ImVec2(ContentRegion.x, 80.0f));
		}
		ImGui::Text("FPS: %.1f", static_cast<double>(1.f / FrameTimeMean));

		ImGui::Text(" ");
		ImGui::Text("Memory");
		ImGui::Text("Number of allocations: %lu", MemoryTracker::Get().GetNumAllocs());
		ImGui::Text("Memory used: %s", StringUtils::CreateBytesString(MemoryTracker::Get().GetMemUsed()).c_str());

		ImGui::Text("GPU");
		ImGui::Separator();
		static GPUInfo GpuInfo = RAL::Get().GetGPUInfo();
		ImGui::Text("Vendor: %s", GpuInfo.Vendor);
		ImGui::Text("Renderer: %s", GpuInfo.Renderer);
		ImGui::Text("Version: %s", GpuInfo.Version);
	}
}