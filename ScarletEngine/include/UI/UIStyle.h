#pragma once

#include "Core/Core.h"

namespace ScarletEngine
{
	/** Static class which stores UI style information */
	class IUIStyle
	{
	public:
		virtual void Initialize() = 0;
		virtual void Shutdown() {};

		Color GetColor(const char* ColorName);
		void SetColor(const char* ColorName, const Color& StyleColor);
	private:
		UnorderedMap<String, Color> Colors;
	};
}