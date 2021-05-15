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

		template <typename ...Components>
		const Array<ProxyType<Components...>>& GetEntities() const
		{
			return Reg->GetProxies<Components...>();
		}

		template <typename ...Components>
		std::optional<ProxyType<Components...>> GetEntity(EID EntityID) const
		{
			return Reg->GetProxy<Components...>(EntityID);
		}

		template <typename SingletonType>
        SingletonType* GetSingleton() const
		{
			return Reg->GetSingleton<std::remove_cv_t<SingletonType>>();
		}

		const String Name;
	private:
		friend class SystemScheduler;
		Registry* Reg = nullptr;
	};

	template <typename... ComponentTypes>
	class System : public ISystem
	{
	public:
		template <typename ...Components>
		Array<ProxyType<Components...>> GetEntities() const
		{
			ZoneScoped
			static_assert(std::conjunction_v<Contains<Components, ComponentTypes...>...>,
				"Trying to get components which are not marked in the system's signature!");

			// Create an array of std::tuples of references to components
			// could probably cache some of this work
			return ISystem::GetEntities<Components...>();
		}

		template <typename ...Components>
		std::optional<ProxyType<Components...>> GetEntity(EID EntityID) const
		{
			ZoneScoped
			static_assert(std::conjunction_v<Contains<Components, ComponentTypes...>...>,
				"Trying to get components which are not marked in the system's signature!");

			return ISystem::GetEntity<Components...>(EntityID);
		}

		template <typename ...Components>
		ProxyType<Components...> GetEntityChecked(EID EntityID) const
		{
			const std::optional<ProxyType<Components...>> OptProxy = GetEntity<Components...>(EntityID);
			check(OptProxy.has_value());
			return OptProxy.value();
		}

		template <typename SingletonType>
		SingletonType* GetSingleton() const
		{
			static_assert(Contains_V<SingletonType, ComponentTypes...>,
				"Trying to get singleton which is not marked in the system's signature!");
			return ISystem::GetSingleton<SingletonType>();
		}
	};
}
