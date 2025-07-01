#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem : public System {
public:
    AnimationSystem() {
        RequireComponent<AnimationComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update() {
        for (auto entity : GetSystemEntities()) {
            auto& animation = entity.GetComponent<AnimationComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();

            auto lastedTime = (SDL_GetTicks() - animation.startTime) / 1000 * animation.speed;
            animation.frameIndex = static_cast<int>(lastedTime) % animation.numFrames;

            sprite.srcRect.x = sprite.width * animation.frameIndex;
            sprite.srcRect.y = 0;
            sprite.srcRect.w = sprite.width;
            sprite.srcRect.h = sprite.height;
        }
    }
};
#endif