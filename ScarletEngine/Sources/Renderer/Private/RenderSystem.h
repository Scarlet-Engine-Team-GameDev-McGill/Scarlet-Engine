#pragma once
#include "System.h"
#include "StaticMeshComponent.h"
#include "Transform.h"

namespace ScarletEngine
{
    class SMCRenderSystem : public System<SceneProxy, const StaticMeshComponent, const Transform>
    {
    public:
        virtual void Update() const override
        {
            SceneProxy* Proxy = GetSingleton<SceneProxy>();
            Proxy->Reset(); // @todo: this will only work while there is a single rendering system
            
            for (const auto& [EID, SMC, Trans] : GetEntities<const StaticMeshComponent, const Transform>())
            {
                Proxy->DrawSMC(*Trans, *SMC);
            }
        }
    };
}