#include "EditorUILayer.h"

#include "Engine.h"
#include "Editor.h"
#include "SceneGraph.h"
#include "EditorViewport.h"
#include "PropertyEditor.h"
#include "OutputLog.h"
#include "Stats.h"
#include "AssetView.h"
#include "ImGuizmo.h"

namespace ScarletEngine
{
	void EditorUILayer::Initialize()
	{
		AddWidget(MakeShared<SceneHierarchyPanel>(GEditor->GetActiveWorld()));
		AddWidget(MakeShared<PropertyEditorPanel>());
		AddWidget(MakeShared<OutputLogPanel>());
		AddWidget(MakeShared<StatsPanel>());
		AddWidget(MakeShared<AssetView>());

		// Create an initial viewport
		AddWidget(MakeShared<EditorViewportPanel>(GEditor->GetActiveWorld()));

		ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
	}

	void EditorUILayer::BeginFrame()
	{
		ImGuizmo::BeginFrame();
	}

	void EditorUILayer::Draw()
	{
		ZoneScoped

		static bool bDockspaceOpen = true;
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

		{
			ZoneScopedN("Editor Dockspace")
			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			ImGuiViewport* ImGuiMainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImGuiMainViewport->Pos);
			ImGui::SetNextWindowSize(ImGuiMainViewport->Size);
			ImGui::SetNextWindowViewport(ImGuiMainViewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (DockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			{
				WindowFlags |= ImGuiWindowFlags_NoBackground;
			}

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace", &bDockspaceOpen, WindowFlags);
			ImGui::PopStyleVar();

			ImGui::PopStyleVar(2);

			ImGuiID DockspaceID = ImGui::GetID("DockSpace");
			ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
		}

		DrawMenuBar();

		ImGui::End(); // end Dockspace

		{
			ZoneScopedN("Demo Window")
			ImGui::ShowDemoWindow();
		}
	}

	void EditorUILayer::DrawMenuBar()
	{
		// Draw the top menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					GEngine->SignalQuit();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Add Viewport"))
				{
					AddViewport();
				}
				if (ImGui::MenuItem("Output Log"))
				{
					// Toggle visibility of output log
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorUILayer::AddViewport()
	{
		AddWidget(MakeShared<EditorViewportPanel>(GEditor->GetActiveWorld()));
	}
}