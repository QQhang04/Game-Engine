#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"

class ProjectileSystem : public System {
public:
    ProjectileSystem() {
        RequireComponent<ProjectileEmitterComponent>();
    }

    void Update(std::unique_ptr<Registry>& registry) {
        for (auto entity : GetSystemEntities()) {
            auto& emitter = entity.GetComponent<ProjectileEmitterComponent>();
            if (emitter.lastEmissionTime + emitter.repeatFrequency < SDL_GetTicks()) {
                // TODO 适用对象池进行优化
                Logger::Log("Create Projectile");
                auto projectile = registry->CreateEntity();

                auto& transform = entity.GetComponent<TransformComponent>();

                glm::vec2 projectilePosition = transform.position;
                if (entity.HasComponent<SpriteComponent>()) {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width / 2);
                    projectilePosition.y += (transform.scale.y * sprite.height / 2);
                }

                projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                projectile.AddComponent<RigidBodyComponent>(emitter.projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(glm::vec2(4.0, 4.0));
                projectile.AddComponent<ProjectileComponent>(emitter.isFriendly, emitter.hitPercentDamage, emitter.projectileDuration);

                emitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};

#endif