#include "RAL.h"

namespace ScarletEngine
{
	UniquePtr<RAL> RAL::Instance = nullptr;
	RenderAPI RAL::API = RenderAPI::Invalid;
}