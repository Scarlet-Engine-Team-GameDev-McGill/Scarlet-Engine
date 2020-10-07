#include "UI/UIStyle.h"

namespace ScarletEngine
{
	Color IUIStyle::GetColor(const char* ColorName)
	{
		return Colors.at(ColorName);
	}

	void IUIStyle::SetColor(const char* ColorName, const Color& StyleColor)
	{
		Colors[ColorName] = StyleColor;
	}
}