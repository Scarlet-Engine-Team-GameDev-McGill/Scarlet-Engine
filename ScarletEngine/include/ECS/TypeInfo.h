#pragma once

#include "Core/CoreUtils.h"

#define INVALID_EID 0
#define INVALID_CTID 0

namespace ScarletEngine
{
	using CTID = uint64_t;
	using EID = uint64_t;

	namespace Internal
	{
		struct ComponentTypeID
		{
			static CTID GetNextTypeID()
			{
				ZoneScoped
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
			ZoneScoped
			static CTID ID = Internal::ComponentTypeID::GetNextTypeID();
			return ID;
		}
	};
}