#include "Utils/StringUtils.h"

namespace ScarletEngine::StringUtils
{
	String CreateBytesString(size_t Size)
	{
		static const char* UnitsArr[] = { "B", "kB", "MB", "GB", "TB" };

		if (Size == 0)
		{
			return "0 B";
		}
			
		// Take the log base 1024 of the Size (using change of base for logarithm)
		const uint32_t Units = static_cast<uint32_t>(std::log10(Size) / std::log10(1024));

		// Always at most 8 characters, + 1 for null byte
		// "xx.xx yy"
		char Buffer[9];
		snprintf(Buffer, 9, "%.2f %s", (Size / std::pow(1024, Units)), UnitsArr[Units]);
		return String(Buffer);
	}

	String PathConcat(const StringView First, const StringView Second)
	{
		String Result(First);
		if (Result.back() == '/' && Second.front() == '/')
		{
			Result.replace(Result.size() - 1, Second.size(), Second);
		}
		else if (Result.back() != '/' && Second.front() != '/')
		{
			Result.append("/").append(Second);
		}
		else
		{
			Result.append(Second);
		}
		return Result;
	}
}
