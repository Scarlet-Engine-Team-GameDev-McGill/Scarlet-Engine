#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	namespace StringUtils
	{
		String CreateBytesString(size_t Size);

		void PathConcat(String& OutResult, const String& Other);
	}
}