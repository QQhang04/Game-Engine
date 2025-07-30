#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include <vector>

#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Events/ProjectileCollisionEvent.h"

// TODO 使用四叉树、扫略裁剪来优化碰撞检测 
// TODO 引入更多碰撞检测的算法 包括连续碰撞检测
// TODO 地图的碰撞系统使用SDF有向距离场算法不使用碰撞体进行优化
class CollisionSystem : public System {
    public:
        CollisionSystem() {
            Logger::Log("CollisionSystem initialized");
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update(std::unique_ptr<EventBus>& eventBus) {
            std::vector<Entity> entities = GetSystemEntities();
            for (int i = 0; i < entities.size(); i++) {
                Entity entity1 = entities[i];
                TransformComponent transform1 = entity1.GetComponent<TransformComponent>();
                BoxColliderComponent boxCollider1 = entity1.GetComponent<BoxColliderComponent>();

                // TODO 使用layer进行过滤
                for (int j = i + 1; j < entities.size(); j++) {
                    Entity entity2 = entities[j];
                    TransformComponent transform2 = entity2.GetComponent<TransformComponent>();
                    BoxColliderComponent boxCollider2 = entity2.GetComponent<BoxColliderComponent>();
                    bool isColliding = CheckAABBCollision(
                        transform1.position.x + boxCollider1.offset.x,
                        transform1.position.y + boxCollider1.offset.y,
                        boxCollider1.size.x * transform1.scale.x,
                        boxCollider1.size.y * transform1.scale.y,
                        transform2.position.x + boxCollider2.offset.x,
                        transform2.position.y + boxCollider2.offset.y,
                        boxCollider2.size.x * transform2.scale.x,
                        boxCollider2.size.y * transform2.scale.y
                    );
                    if (isColliding) {
                        eventBus->EmitEvent<CollisionEvent>(entity1, entity2);
                        if (entity1.GetGroup() == "projectile") {
                            eventBus->EmitEvent<ProjectileCollisionEvent>(entity1, entity2);
                        }
                        else if (entity2.GetGroup() == "projectile") {
                            eventBus->EmitEvent<ProjectileCollisionEvent>(entity2, entity1);
                        }
                    }
                }
            }
        }

    private:
        bool CheckAABBCollision(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh) {
            return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
        }
};

#endif