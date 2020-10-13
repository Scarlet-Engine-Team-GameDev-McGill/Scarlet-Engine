#pragma once

#include "UIStyle.h"

namespace ScarletEngine
{
	class CoreUIStyle final : public IUIStyle
	{
	public:
		virtual void Initialize() override;
	};
}