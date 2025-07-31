#ifndef RENDER_TEXT_SYSTEM_H
#define RENDER_TEXT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL_ttf.h>

class RenderTextSystem : public System {
    public:
        RenderTextSystem() {
            RequireComponent<TextLabelComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            for (auto entity : GetSystemEntities()) {
                const auto& textLabel = entity.GetComponent<TextLabelComponent>();
                
                 SDL_Surface* surface = TTF_RenderText_Blended(
                    assetStore->GetFont(textLabel.assetId),
                    textLabel.text.c_str(),
                    textLabel.color
                );
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                int labelWidth, labelHeight;
                SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);

                SDL_Rect dstRect = {
                    static_cast<int>(textLabel.position.x),
                    static_cast<int>(textLabel.position.y),
                    labelWidth,
                    labelHeight
                };

                SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
                SDL_DestroyTexture(texture);
            }
        }
};
#endif