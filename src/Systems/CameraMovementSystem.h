#ifndef CAMERA_MOVEMENT_SYSTEM_H
#define CAMERA_MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/TargetCameraComponent.h"

#include <SDL2/SDL.h>
#include "../Game/Game.h"

class CameraMovementSystem : public System {
public:
    CameraMovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<TargetCameraComponent>();
    }

    void Update(SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            const auto& transform = entity.GetComponent<TransformComponent>();
            
            if (transform.position.x + (camera.w / 2) < Game::mapWidth) {
                camera.x = transform.position.x - (Game::windowWidth / 2);
            }

            if (transform.position.y + (camera.h / 2) < Game::mapHeight) {
                camera.y = transform.position.y - (Game::windowHeight / 2);
            }

            // Keep camera rectangle view inside the screen limits
            camera.x = camera.x < 0 ? 0 : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y;
            camera.x = camera.x > camera.w ? camera.w : camera.x;
            camera.y = camera.y > camera.h ? camera.h : camera.y;
        }
    }
};

#endif