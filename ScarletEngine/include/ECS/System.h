#pragma once

#include <type_traits>
#include <functional>
#include "TypeInfo.h"
#include "Registry.h"

namespace ScarletEngine
{
	class Registry;

	class ISystem
	{
	public:
		ISystem(Registry* InReg) : Reg(InReg) {}

		virtual void Run(EID EntityID) const = 0;
		virtual ~ISystem() {}

		Registry* Reg;
	};

	template <typename... ComponentTypes>
	class System : public ISystem
	{
	public:
		using ForEachFunctionType = std::function<void(EID EntityID, std::add_lvalue_reference_t<ComponentTypes>...)>;

		System(Registry* InReg, const ForEachFunctionType& InForEach)
			: ISystem(InReg)
			, ForEach(InForEach)
		{}

		virtual void Run(EID EntityID) const override
		{
			// If our entity has the specified components
			if((Reg->HasComponent<std::remove_cv_t<ComponentTypes>>(EntityID) && ...))
			{
				ForEach(EntityID, *Reg->GetComponent<std::remove_cv_t<ComponentTypes>>(EntityID)...);
			}
		}
	
		ForEachFunctionType ForEach;
	};
}