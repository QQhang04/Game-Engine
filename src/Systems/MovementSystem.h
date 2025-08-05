#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/ObstacleCollisionEvent.h"

class MovementSystem : public System {
    public:   
        MovementSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->Subscribe<ObstacleCollisionEvent>(this, &MovementSystem::OnObstacleCollision);
        }

        void OnObstacleCollision(ObstacleCollisionEvent& event) {
            Entity entity = event.target;
            if (!entity.BelongToGroup("enemy")) {
                return;
            }
            if (!entity.HasComponent<SpriteComponent>() || !entity.HasComponent<RigidBodyComponent>()) {
                return;
            }
            auto& sprite = entity.GetComponent<SpriteComponent>();
            auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
            rigidBody.velocity.x = -rigidBody.velocity.x;
            rigidBody.velocity.y = -rigidBody.velocity.y;
            sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }

        void Update(double deltaTime) {
            for (auto entity : GetSystemEntities()) {
                TransformComponent& transform = entity.GetComponent<TransformComponent>();
                auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

                transform.position += rigidBody.velocity * static_cast<float>(deltaTime);

                bool isOutOfMap = transform.position.x < 0 || transform.position.x > Game::mapWidth 
                                || transform.position.y < 0 || transform.position.y > Game::mapHeight;
                if (isOutOfMap && !entity.HasTag("player")) {
                    entity.Destroy();
                    continue;
                }

                if (entity.HasTag("player")) {
                    int paddingLeft = 10;
                    int paddingTop = 10;
                    int paddingRight = 50;
                    int paddingBottom = 50;
                    transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
                    transform.position.x = transform.position.x > Game::mapWidth - paddingRight ? Game::mapWidth - paddingRight : transform.position.x;
                    transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;
                    transform.position.y = transform.position.y > Game::mapHeight - paddingBottom ? Game::mapHeight - paddingBottom : transform.position.y;
                }
            }
        }
};

#endif