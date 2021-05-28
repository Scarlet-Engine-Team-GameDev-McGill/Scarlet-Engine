#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	struct Color
	{
	public:
		Color() : R(1), G(1), B(1), A(1) {}

		Color(float R, float G, float B)
			: R(R), G(G), B(B), A(1)
		{}

		Color(float R, float G, float B, float A)
			: R(R), G(G), B(B), A(A)
		{}

		/** Convert the color to a glm::vec4 */
		inline glm::vec4 AsVec4() const { return glm::vec4(R, G, B, A); }

		float R;
		float G;
		float B;
		float A;
	};
}