#ifndef PROJECTILE_COLLISION_EVENT_H
#define PROJECTILE_COLLISION_EVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"

class ProjectileCollisionEvent : public Event {
public:
    Entity projectile;
    Entity target;
    ProjectileCollisionEvent(Entity projectile, Entity target) : projectile(projectile), target(target) {}
};

#endif