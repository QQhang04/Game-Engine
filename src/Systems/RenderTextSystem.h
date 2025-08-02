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
                auto& textLabel = entity.GetComponent<TextLabelComponent>();
                if (textLabel.dirty) {
                    if (textLabel.texture) {
                        SDL_DestroyTexture(textLabel.texture);
                        textLabel.texture = nullptr;
                    }
                    SDL_Surface* surface = TTF_RenderText_Blended(
                        assetStore->GetFont(textLabel.assetId),
                        textLabel.text.c_str(),
                        textLabel.color
                    );
                    textLabel.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    textLabel.dirty = false;
                }
                if (textLabel.texture) {
                    int labelWidth, labelHeight;
                    SDL_QueryTexture(textLabel.texture, nullptr, nullptr, &labelWidth, &labelHeight);
                    SDL_Rect dstRect = {
                        static_cast<int>(textLabel.position.x),
                        static_cast<int>(textLabel.position.y),
                        labelWidth,
                        labelHeight
                    };
                    SDL_RenderCopy(renderer, textLabel.texture, nullptr, &dstRect);
                }
            }
        }
};
#endif