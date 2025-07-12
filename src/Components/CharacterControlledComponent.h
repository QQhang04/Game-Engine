#ifndef CHARACTER_CONTROLLED_COMPONENT_H
#define CHARACTER_CONTROLLED_COMPONENT_H

#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

struct CharacterControlledComponent
{
    float speed;
    CharacterControlledComponent(float speed = 1.0f) : speed(speed) {}
};

#endif