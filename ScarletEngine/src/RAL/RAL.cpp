#include "RAL/RAL.h"

namespace ScarletEngine
{
	std::unique_ptr<RAL> RAL::Instance = nullptr;
	RenderAPI RAL::API = RenderAPI::Invalid;
}