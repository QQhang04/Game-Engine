#ifndef RENDERHEALTHBARSYSTEM_H
#define RENDERHEALTHBARSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/HealthComponent.h"
#include <SDL2/SDL.h>

// TODO 模仿智能指针的思路,在没有一个人的时候思路
class RenderHealthBarSystem: public System {
    public:
        RenderHealthBarSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<HealthComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();
                const auto health = entity.GetComponent<HealthComponent>();

                bool isOutsideCameraView = (
                    transform.position.x + (transform.scale.x * sprite.width) < camera.x ||
                    transform.position.x > camera.x + camera.w ||
                    transform.position.y + (transform.scale.y * sprite.height) < camera.y ||
                    transform.position.y > camera.y + camera.h
                );
                
                if (isOutsideCameraView) {
                    continue;
                }

                // 渲染血条
                SDL_Color healthBarColor = {255, 255, 255};

                if (health.healthPercentage >= 0 && health.healthPercentage < 40) {
                    // 0-40 = red
                    healthBarColor = {255, 0, 0};
                }
                if (health.healthPercentage >= 40 && health.healthPercentage < 80) {
                    // 40-80 = yellow
                    healthBarColor = {255, 255, 0};
                }
                if (health.healthPercentage >= 80 && health.healthPercentage <= 100) {
                    // 80-100 = green
                    healthBarColor = {0, 255, 0};
                }

                int healthBarWidth = 15;
                int healthBarHeight = 3;
                double healthBarPosX = (transform.position.x + (sprite.width * transform.scale.x)) - camera.x;
                double healthBarPosY = (transform.position.y) - camera.y;

                SDL_Rect healthBarRectangle = {
                    static_cast<int>(healthBarPosX),
                    static_cast<int>(healthBarPosY),
                    static_cast<int>(healthBarWidth * (health.healthPercentage / 100.0)),
                    static_cast<int>(healthBarHeight)
                };
                SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
                SDL_RenderFillRect(renderer, &healthBarRectangle);

                // 渲染血量文本
                SDL_Texture* texture = GetHealthTextTexture(health.healthPercentage, renderer, assetStore, healthBarColor);

                int labelWidth = 0;
                int labelHeight = 0;
                SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);
                SDL_Rect healthBarTextRectangle = {
                    static_cast<int>(healthBarPosX),
                    static_cast<int>(healthBarPosY) + 5,
                    labelWidth,
                    labelHeight
                };
                
                SDL_RenderCopy(renderer, texture, NULL, &healthBarTextRectangle);
            }
        }

        void OnRemove() {
            Logger::Log("RenderHealthBarSystem OnRemove");
            for (auto& texture : healthTextTextureCache) {
                SDL_DestroyTexture(texture.second);
            }
            healthTextTextureCache.clear();
        }

    private:
    // 增加血量数字texture缓存池
    std::unordered_map<int, SDL_Texture*> healthTextTextureCache;

    SDL_Texture* GetHealthTextTexture(int healthValue, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Color color) {
        // 如果缓存中已存在，直接返回
        if (healthTextTextureCache.count(healthValue) > 0 && healthTextTextureCache[healthValue] != nullptr) {
            return healthTextTextureCache[healthValue];
        }

        // 否则创建新的texture并缓存
        std::string healthText = std::to_string(healthValue);
        SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont("pico8-font-5"), healthText.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        healthTextTextureCache[healthValue] = texture;
        return texture;
    }
};

#endif
