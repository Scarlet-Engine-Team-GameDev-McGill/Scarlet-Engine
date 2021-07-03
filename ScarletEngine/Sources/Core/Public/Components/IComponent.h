#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
    /**
    * Base class for all component types.
    * IComponent is just a skeleton which provides a single method to retrieve its static, portable type id. 
    */
    class IComponent
    {
    };

#define COMPONENT_DEFINITION(ComponentTypeName)                     \
    public:                                                         \
    constexpr static CTID StaticTypeID = COMPILE_TIME_CRC32_STR(#ComponentTypeName);\
    const static inline String StaticTypeName = #ComponentTypeName; 
}