#ifndef CHARACTER_CONTROLLED_COMPONENT_H
#define CHARACTER_CONTROLLED_COMPONENT_H

#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

enum class Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    NONE = -1
};

struct CharacterControlledComponent
{
    float speed;
    Direction currentDirection;
    
    CharacterControlledComponent(float speed = 1.0f) : speed(speed), currentDirection(Direction::NONE) {}
};

#endif