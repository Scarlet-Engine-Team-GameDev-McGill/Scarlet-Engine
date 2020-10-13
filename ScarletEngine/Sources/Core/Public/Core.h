#pragma once

/** This is where we put all high level headers which would be included by a game or the editor */

#include "CoreMinimal.h"
#include "Engine.h"
#include "ITickable.h"
#include "Event.h"
#include "Delegate.h"
#include "Archive.h"
#include "Color.h"
#include "Utils/StringUtils.h"
#include "IModule.h"
#include "ModuleManager.h"

// Core component types
#include "Transform.h"

namespace ScarletEngine
{
	extern UniquePtr<Engine> GEngine;
}