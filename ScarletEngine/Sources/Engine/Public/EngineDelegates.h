#pragma once

#include "CoreMinimal.h"
#include "World.h"

namespace ScarletEngine
{
    struct EngineDelegates
    {
    public:
        static Event<const SharedPtr<World>&> OnWorldChanged;
    };
}