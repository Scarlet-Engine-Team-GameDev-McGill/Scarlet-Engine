#pragma once

#include "Core.h"
#include "TypeInfo.h"
#include "Registry.h"

template <class T, class... U>
struct Contains : std::disjunction<std::is_same<T, U>...>{};
template <class... Traits>
constexpr bool Contains_V = Contains<Traits...>::value;

namespace ScarletEngine
{
	class Registry;

	class ISystem
	{
	public:
		virtual ~ISystem() {}

		virtual void Update() const {}
		virtual void FixedUpdate() const {}

		Registry* Reg;
		const String Name;
	};

	template <typename... ComponentTypes>
	class System : public ISystem
	{
	public:
		template <typename ...Components>
		using ProxyType = std::tuple<EID, std::add_pointer_t<Components>...>;
		
		template <typename ...Components>
		Array<ProxyType<Components...>> GetEntities() const
		{
			ZoneScoped
			static_assert(std::conjunction_v<Contains<Components, ComponentTypes...>...>,
				"Trying to get components which are not marked in the system's signature!");

			// Create an array of std::tuples of references to components
			// could probably cache some of this work
			Array<ProxyType<Components...>> EntityProxies;

			for (const EID Entity : Reg->GetEntities())
			{
				if ((Reg->HasComponent<std::remove_cv_t<Components>>(Entity) && ...))
				{
					EntityProxies.push_back(std::make_tuple(Entity, Reg->GetComponent<std::remove_cv_t<Components>>(Entity)...));
				}
			}

			return EntityProxies;
		}

		template <typename SingletonType>
		SingletonType* GetSingleton() const
		{
			static_assert(Contains_V<SingletonType, ComponentTypes...>,
				"Trying to get singleton which is not marked in the system's signature!");
			return Reg->GetSingleton<std::remove_cv_t<SingletonType>>();
		}
	};

	template <typename Type>
	concept ECSSystem = std::derived_from<Type, ISystem>;

}