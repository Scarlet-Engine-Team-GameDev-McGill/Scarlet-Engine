#pragma once

#include "Core/CoreUtils.h"

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
				static CTID NextID = 0;
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