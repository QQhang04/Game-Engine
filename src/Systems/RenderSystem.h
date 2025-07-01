#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>

class RenderSystem : public System {
    public:   
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            struct RenderableEntity {
                TransformComponent transform;
                SpriteComponent sprite;
                
                RenderableEntity(const TransformComponent& t, const SpriteComponent& s)
                    : transform(t), sprite(s) {}
            };
            std::vector<RenderableEntity> renderables;
            for (auto entity : GetSystemEntities()) {
                renderables.emplace_back(
                    entity.GetComponent<TransformComponent>(),
                    entity.GetComponent<SpriteComponent>()
                );
            }

            // TODO 优化排序性能，调用时机
            // sort by zIndex 越小越先渲染
            std::sort(renderables.begin(), renderables.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
                return a.sprite.zIndex < b.sprite.zIndex;
            });
            
            for (auto& renderable : renderables) {
                const auto transform = renderable.transform;
                const auto sprite = renderable.sprite;

                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect destRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };
                
                SDL_RenderCopyEx(renderer, 
                assetStore->GetTexture(sprite.textureId), 
                &srcRect, 
                &destRect,
                transform.rotation,
                nullptr,
                SDL_FLIP_NONE);
            }
        }
};

#endif