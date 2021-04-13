#pragma once

#include "Core.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	namespace Achilles
	{
		struct RigidBodyComponent;
	}

	struct EntityHandle;

	class PropertyEditorPanel : public UIWindow
	{
	public:
		PropertyEditorPanel();

		virtual void Construct() override;
		virtual void DrawWindowContent() override;

		void OnSelectionChanged();
		void OnSelectionCleared();

	private:
		void DrawTransformWidget(TransformComponent& Transform) const;
		void DrawRigidBodyWidget(Achilles::RigidBodyComponent& RigidBody) const;
		
	private:
		EntityHandle* FocusedEntity;
	};
}
