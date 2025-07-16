#ifndef RENDER_BOX_COLLIDER_SYSTEM_H
#define RENDER_BOX_COLLIDER_SYSTEM_H

#include <SDL2/SDL.h>
#include "../../ECS/ECS.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Log/Logger.h"
#include <set>

#include "../../EventBus/EventBus.h"
#include "../../Events/CollisionEvent.h"

class RenderBoxColliderSystem : public System {
    private:
        std::set<int> collidingEntities;

    public:
        RenderBoxColliderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->Subscribe<CollisionEvent>(this, &RenderBoxColliderSystem::OnCollision);
        }

        void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
            std::vector<Entity> entities = GetSystemEntities();
            for (int i = 0; i < entities.size(); i++) {
                Entity entity = entities[i];
                TransformComponent transform = entity.GetComponent<TransformComponent>();
                BoxColliderComponent boxCollider = entity.GetComponent<BoxColliderComponent>();

                SDL_Rect collider_rect = {
                    static_cast<int>(transform.position.x + boxCollider.offset.x - camera.x),
                    static_cast<int>(transform.position.y + boxCollider.offset.y - camera.y),
                    static_cast<int>(boxCollider.size.x * transform.scale.x),
                    static_cast<int>(boxCollider.size.y * transform.scale.y)
                };
                
                // 如果实体在碰撞集合中，设置为黄色，否则为红色
                if (collidingEntities.find(entity.GetId()) != collidingEntities.end()) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                }
                SDL_RenderDrawRect(renderer, &collider_rect);
            }
            // 每帧清除碰撞记录，这样只有持续碰撞的才会保持黄色
            collidingEntities.clear();
        }

        void OnCollision(CollisionEvent& event) {
            Logger::LogEvent("on collision for entities " + std::to_string(event.entityA.GetId()) + " and " + std::to_string(event.entityB.GetId()));
            // 记录碰撞的实体
            collidingEntities.insert(event.entityA.GetId());
            collidingEntities.insert(event.entityB.GetId());
        }
};

#endif