#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileLifecycleSystem : public System {
public:
    ProjectileLifecycleSystem() {
        RequireComponent<ProjectileComponent>();
    }

    void Update(std::unique_ptr<Registry>& registry) {
        for (auto entity : GetSystemEntities()) {
            auto& projectile = entity.GetComponent<ProjectileComponent>();
            if (projectile.duration < SDL_GetTicks() - projectile.startTime) {
                registry->RemoveEntity(entity);
            }
        }
    }
};

#endif