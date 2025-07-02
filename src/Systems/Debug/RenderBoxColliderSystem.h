#ifndef RENDER_BOX_COLLIDER_SYSTEM_H
#define RENDER_BOX_COLLIDER_SYSTEM_H

#include <SDL2/SDL.h>
#include "../../ECS/ECS.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"

class RenderBoxColliderSystem : public System {
    public:
        RenderBoxColliderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update(SDL_Renderer* renderer) {
            std::vector<Entity> entities = GetSystemEntities();
            for (int i = 0; i < entities.size(); i++) {
                Entity entity = entities[i];
                TransformComponent transform = entity.GetComponent<TransformComponent>();
                BoxColliderComponent boxCollider = entity.GetComponent<BoxColliderComponent>();

                SDL_Rect collider_rect = {
                    static_cast<int>(transform.position.x + boxCollider.offset.x),
                    static_cast<int>(transform.position.y + boxCollider.offset.y),
                    static_cast<int>(boxCollider.size.x * transform.scale.x),
                    static_cast<int>(boxCollider.size.y * transform.scale.y)
                };
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &collider_rect);
            }
        }
};

#endif