#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CharacterControlledComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Events/KeyPressedEvent.h"
#include <glm/glm.hpp>

class ProjectileSystem : public System {
private:
    glm::vec2 GetDirectionVector(Direction dir) {
        switch(dir) {
            case Direction::UP: return glm::vec2(0.0f, -1.0f);
            case Direction::DOWN: return glm::vec2(0.0f, 1.0f);
            case Direction::LEFT: return glm::vec2(-1.0f, 0.0f);
            case Direction::RIGHT: return glm::vec2(1.0f, 0.0f);
            case Direction::NONE: return glm::vec2(0.0f, 0.0f);
            default: return glm::vec2(0.0f, 0.0f);
        }
    }

public:
    ProjectileSystem() {
        RequireComponent<ProjectileEmitterComponent>();
    }

     void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<KeyPressedEvent>(this, &ProjectileSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event) {
        if (event.symbol == SDLK_SPACE) {
            Logger::Log("Space key pressed");
            // TODO 如何快速拿到player而不是遍历
            for (auto entity : GetSystemEntities()) {
                if (entity.HasComponent<CharacterControlledComponent>()) {
                    const auto& characterControlled = entity.GetComponent<CharacterControlledComponent>();
                    auto& emitter = entity.GetComponent<ProjectileEmitterComponent>();
                    if (emitter.lastEmissionTime + emitter.repeatFrequency < SDL_GetTicks()) {
                        Logger::Log("Create Projectile");
                        auto projectile = entity.registry->CreateEntity();

                        auto& transform = entity.GetComponent<TransformComponent>();

                        glm::vec2 projectilePosition = transform.position;
                        if (entity.HasComponent<SpriteComponent>()) {
                            const auto sprite = entity.GetComponent<SpriteComponent>();
                            projectilePosition.x += (transform.scale.x * sprite.width / 2);
                            projectilePosition.y += (transform.scale.y * sprite.height / 2);
                        }
                        // 根据角色方向计算导弹速度向量
                        glm::vec2 directionVector = GetDirectionVector(characterControlled.currentDirection);
                        float speed = glm::length(emitter.projectileVelocity);
                        glm::vec2 projectileVelocity = directionVector * speed;

                        projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                        projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                        projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
                        projectile.AddComponent<BoxColliderComponent>(glm::vec2(4.0, 4.0));
                        projectile.AddComponent<ProjectileComponent>(emitter.isFriendly, emitter.hitPercentDamage, emitter.projectileDuration);

                        emitter.lastEmissionTime = SDL_GetTicks();
                    }
                }
            }
        }
    }

    void Update(std::unique_ptr<Registry>& registry) {
        for (auto entity : GetSystemEntities()) {
            if (entity.HasComponent<CharacterControlledComponent>()) {
                continue;
            }
            auto& emitter = entity.GetComponent<ProjectileEmitterComponent>();
            if (emitter.lastEmissionTime + emitter.repeatFrequency < SDL_GetTicks()) {
                // TODO 用对象池进行优化
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