#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	class IUIStyle;

	class UIStyleRegistry
	{
	public:
		void RegisterStyle(const String& Name, const SharedPtr<IUIStyle>& Style);
		void UnregisterStyle(const String& Name);

		IUIStyle* GetStyle(const String& Name) const;

		static UIStyleRegistry& Get() { static UIStyleRegistry Instance; return Instance; }
	private:
		UnorderedMap<String, SharedPtr<IUIStyle>> Styles;
	};
}