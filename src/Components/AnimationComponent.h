#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <SDL2/SDL.h>
struct AnimationComponent {
    int numFrames;
    int speed;
    int frameIndex;
    float startTime;

    AnimationComponent(int numFrames = 1, int speed = 1) : numFrames(numFrames), speed(speed), frameIndex(0) {
        startTime = SDL_GetTicks();
    }
};
#endif