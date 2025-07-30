#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Events/ProjectileCollisionEvent.h"

class DamageSystem : public System {
    public:
        DamageSystem() {
            RequireComponent<HealthComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->Subscribe<ProjectileCollisionEvent>(this, &DamageSystem::OnProjectileCollision);
        }

        void OnProjectileCollision(ProjectileCollisionEvent& event) {
            auto projectile = event.projectile;
            auto target = event.target;
            // 不处理子弹打子弹
            if (target.BelongToGroup("projectile")) {
                return;
            }

            // 目标必须有血量
            if (!target.HasComponent<HealthComponent>()) {
                return;
            }

            ProjectileComponent projectileComponent = projectile.GetComponent<ProjectileComponent>();
            bool isFriendly = projectileComponent.isFriendly;

            bool isTargetPlayer = target.HasTag("player");
            bool isTargetEnemy = target.BelongToGroup("enemy");

            // 友方子弹只能打敌人，敌方子弹只能打玩家
            if ((isFriendly && !isTargetEnemy) || (!isFriendly && !isTargetPlayer)) {
                return;
            }

            auto& healthComponent = target.GetComponent<HealthComponent>();
            healthComponent.healthPercentage -= projectileComponent.hitPercentDamage;
            Logger::Log(std::to_string(healthComponent.healthPercentage));

            if (healthComponent.healthPercentage <= 0) {
                target.Destroy();
            }
            projectile.Destroy();
        }
};

#endif