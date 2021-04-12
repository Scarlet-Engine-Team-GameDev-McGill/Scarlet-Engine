#pragma once

#include "CoreUtils.h"

namespace ScarletEngine
{
	using CTID = uint32_t;
	using EID = uint32_t;

	namespace Internal
	{
		struct ComponentTypeID
		{
			static CTID GetNextTypeID()
			{
				static CTID NextID = 1;
				return NextID++;
			}
		};
	}

	/** Get a unique type ID for each template instantiation */
	template <typename ComponentType>
	struct ComponentTypeID
	{
		static CTID Value()
		{
			static CTID ID = Internal::ComponentTypeID::GetNextTypeID();
			return ID;
		}
	};
}

#define INVALID_EID static_cast<CTID>(0)
#define INVALID_CTID static_cast<EID>(0)
