#pragma once

namespace ScarletEngine
{
	class UIWidget
	{
	public:
		virtual void Initialize() {};
		virtual void Draw() = 0;
		virtual void Destroy() {};
	};
}