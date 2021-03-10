#pragma once

#include "Core.h"
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

		virtual ~ISystem() {}

		virtual void Update(double DeltaTime, EID EntityID) const {}
		virtual void FixedUpdate(double DeltaTime, EID EntityID) const {}

		Registry* Reg;
		String Name;
	};

	template <typename... ComponentTypes>
	class System : public ISystem
	{
	public:
		using ForEachFunctionType = std::function<void(double DeltaTime, EID EntityID, std::add_lvalue_reference_t<ComponentTypes>...)>;

		System(Registry* InReg, const String& InName)
			: ISystem(InReg, InName)
			, ForEach()
		{}

		virtual void Update(double DeltaTime, EID EntityID) const override
		{
			// If our entity has the specified components
			if((Reg->HasComponent<std::remove_cv_t<ComponentTypes>>(EntityID) && ...))
			{
				ForEach(DeltaTime, EntityID, *Reg->GetComponent<std::remove_cv_t<ComponentTypes>>(EntityID)...);
			}
		}

		virtual void FixedUpdate(double DeltaTime, EID EntityID) const override
		{
			ZoneScoped
			// If our entity has the specified components
			if ((Reg->HasComponent<std::remove_cv_t<ComponentTypes>>(EntityID) && ...))
			{
				ForEach(DeltaTime, EntityID, *Reg->GetComponent<std::remove_cv_t<ComponentTypes>>(EntityID)...);
			}
		}

		inline void Each(const ForEachFunctionType& InForEach)
		{
			ForEach = InForEach;
		}
	
		ForEachFunctionType ForEach;

	};
}