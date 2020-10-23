#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	namespace StringUtils
	{
		String CreateBytesString(size_t Size);

		// #todo_Core: implement a version of this which returns a string.
		void PathConcat(String& OutResult, const String& Other);
	}
}