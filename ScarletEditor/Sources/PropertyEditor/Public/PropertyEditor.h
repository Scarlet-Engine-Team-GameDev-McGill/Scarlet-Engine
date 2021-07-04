#pragma once

#include "Core.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	namespace Achilles
	{
		class RigidBodyComponent;
	}

	class Entity;

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

		void DrawComponentFactoryList() const;

	private:
		Entity* FocusedEntity;
	};
}
