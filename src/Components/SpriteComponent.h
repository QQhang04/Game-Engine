#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <string>

struct SpriteComponent {
    std::string textureId;
    int width, height;

    SpriteComponent(const std::string& textureId = "", int w = 0, int h = 0) : textureId(textureId), width(w), height(h) {}
};

#endif