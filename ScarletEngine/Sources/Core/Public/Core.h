#pragma once

/** This is where we put all high level headers which would be included by a game or the editor */

#include "CoreMinimal.h"
#include "Event.h"
#include "Delegate.h"
#include "Serialization/Archive.h"
#include "Serialization/BinaryArchive.h"
#include "Serialization/JsonArchive.h"
#include "Color.h"
#include "Utils/StringUtils.h"
#include "IModule.h"
#include "ModuleManager.h"
#include "InputCodes.h"
#include "Components/IComponent.h"
#include "Reflection/Reflection.h"

// Core component types
#include "Components/TransformComponent.h"
