#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent {
    std::string textureId;
    int width, height;
    int zIndex;
    bool isFixed;
    SDL_Rect srcRect;
    SDL_RendererFlip flip;

    SpriteComponent(const std::string& textureId = "", int w = 0, int h = 0, int zIndex = 0, int srcRectX = 0, int srcRectY = 0, bool isFixed = false) : textureId(textureId), width(w), height(h), zIndex(zIndex), isFixed(isFixed) {
        srcRect.x = srcRectX;
        srcRect.y = srcRectY;
        srcRect.w = width;
        srcRect.h = height;
        flip = SDL_FLIP_NONE;
    }
};

#endif