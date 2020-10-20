#include "UIStyleRegistry.h"
#include "UIStyle.h"

namespace ScarletEngine
{
	void UIStyleRegistry::RegisterStyle(const String& Name, const SharedPtr<IUIStyle>& Style)
	{
		Styles[Name] = Style;
		Style->Initialize();
	}

	void UIStyleRegistry::UnregisterStyle(const String& Name)
	{
		if (auto It = Styles.find(Name); It != Styles.end())
		{
			It->second->Shutdown();
			Styles.erase(It);
		}
	}

	IUIStyle* UIStyleRegistry::GetStyle(const String& Name) const
	{
		return Styles.find(Name) != Styles.end() ? Styles.at(Name).get() : nullptr;
	}
}
