#pragma once

#include "Core/Core.h"
#include "TypeInfo.h"
#include "Registry.h"

namespace ScarletEngine
{
	class Registry;

	class ISystem
	{
	public:
		ISystem(Registry* InReg, const String& InName)
			: Reg(InReg)
			, Name(InName) 
		{}

		virtual void Run(EID EntityID) const = 0;
		virtual ~ISystem() {}

		Registry* Reg;
		String Name;
	};

	template <typename... ComponentTypes>
	class System : public ISystem
	{
	public:
		using ForEachFunctionType = std::function<void(EID EntityID, std::add_lvalue_reference_t<ComponentTypes>...)>;

		System(Registry* InReg, const String& InName)
			: ISystem(InReg, InName)
			, ForEach()
		{}

		virtual void Run(EID EntityID) const override
		{
			// If our entity has the specified components
			if((Reg->HasComponent<std::remove_cv_t<ComponentTypes>>(EntityID) && ...))
			{
				ForEach(EntityID, *Reg->GetComponent<std::remove_cv_t<ComponentTypes>>(EntityID)...);
			}
		}

		void Each(const ForEachFunctionType& InForEach)
		{
			ForEach = InForEach;
		}
	
		ForEachFunctionType ForEach;
	};
}