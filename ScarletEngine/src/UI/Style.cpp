#include "UI/Style.h"

namespace ScarletEngine
{
	UnorderedMap<String, Color> Style::Colors;

	Color Style::GetColor(const char* ColorName)
	{
		return Colors.at(ColorName);
	}

	void Style::SetColor(const char* ColorName, const Color& StyleColor)
	{
		Colors[ColorName] = StyleColor;
	}
}