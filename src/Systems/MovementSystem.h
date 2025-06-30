#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
    public:   
        MovementSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();

        }
        void Update(double deltaTime) {
            for (auto entity : GetSystemEntities()) {
                TransformComponent& transform = entity.GetComponent<TransformComponent>();
                auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

                transform.position += rigidBody.velocity * static_cast<float>(deltaTime);
            }
        }
};

#endif