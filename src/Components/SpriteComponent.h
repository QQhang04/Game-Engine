#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

struct SpriteComponent {
    int width, height;

    SpriteComponent(int w = 0, int h = 0) : width(w), height(h) {}
};

#endif