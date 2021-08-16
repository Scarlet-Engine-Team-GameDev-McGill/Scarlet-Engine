#pragma once

#include "Core.h"
#include "ITickable.h"
#include "SceneProxy.h"
#include "SystemScheduler.h"
#include "Entity.h"


namespace ScarletEngine
{
    using OnEntityAddedToWorldEvent = Event<const EntityPtr&>;

    class World final : public ITickable
    {
    public:
        World();
        virtual ~World() {}

        /* ITickable interface */
        virtual void Tick(double DeltaTime) override;
        virtual void FixedTick(double DeltaTime) override;

        /** Returns the render scene proxy for this world */
        // #todo_rendering: should be managed by the renderer directly
        SceneProxy* GetRenderSceneProxy() const { return Reg.GetSingleton<SceneProxy>(); }

        /** Create a new entity in the world with specified component types */
        template <typename... ComponentTypes>
        std::tuple<EID, std::add_pointer_t<ComponentTypes>...> CreateEntity(const char* Name)
        {
            const auto EntityProxy = Reg.CreateEntity<ComponentTypes...>();
            EntityPtr& Ent = Entities.emplace_back(ScarNew(Entity, Name, std::get<EID>(EntityProxy), this));

            OnEntityAddedToWorld.Broadcast(Ent);
            return EntityProxy;
        }

        const Array<EntityPtr>& GetEntities() const { return Entities; }

        template <typename ComponentType>
        ComponentType* GetComponent(const EID& EntityID) const
        {
            return Reg.GetComponent<ComponentType>(EntityID);
        }

        template <typename ComponentType>
        ComponentType* AddComponent(const EID EntityID)
        {
            return Reg.AddComponent<ComponentType>(EntityID);
        }

        template <typename ComponentType>
        bool RemoveComponent(const EID EntityID)
        {
            return Reg.RemoveComponent<ComponentType>(EntityID);
        }

        OnEntityAddedToWorldEvent& GetOnEntityAddedToWorldEvent() { return OnEntityAddedToWorld; }

        void SetWorldName(const String& InNewName) { WorldName = InNewName; }
        const String& GetWorldName() const { return WorldName; }

        // #todo_core: hacky solution. should be made private again
        Registry& GetRegistry() { return Reg; }
    private:
        Registry Reg;
        Array<EntityPtr> Entities;
        String WorldName;

        OnEntityAddedToWorldEvent OnEntityAddedToWorld;
    };
}
