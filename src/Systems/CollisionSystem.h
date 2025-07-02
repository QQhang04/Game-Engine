#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include <vector>

class CollisionSystem : public System {
    public:
        CollisionSystem() {
            Logger::Log("CollisionSystem initialized");
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update() {
            std::vector<Entity> entities = GetSystemEntities();
            for (int i = 0; i < entities.size(); i++) {
                Entity entity1 = entities[i];
                TransformComponent transform1 = entity1.GetComponent<TransformComponent>();
                BoxColliderComponent boxCollider1 = entity1.GetComponent<BoxColliderComponent>();

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
                        // TODO: 发起事件通知碰撞发生
                        Logger::LogCollision("Collision detected between entities " + std::to_string(entity1.GetId()) + " and " + std::to_string(entity2.GetId()));
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