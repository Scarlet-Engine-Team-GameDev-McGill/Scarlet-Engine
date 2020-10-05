#pragma once

#include "Core/Core.h"

namespace ScarletEngine
{
	/** Static class which stores UI style information */
	class Style
	{
	public:
		static Color GetColor(const char* ColorName);
		static void SetColor(const char* ColorName, const Color& StyleColor);
	private:
		static UnorderedMap<String, Color> Colors;
	};
}