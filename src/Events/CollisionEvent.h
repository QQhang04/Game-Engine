#ifndef COLLISION_EVENT_H
#define COLLISION_EVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"

class CollisionEvent : public Event {
public:
    Entity entityA;
    Entity entityB;
    CollisionEvent(Entity entityA, Entity entityB) : entityA(entityA), entityB(entityB) {}
};

#endif