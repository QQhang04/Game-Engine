#ifndef OBSTACLE_COLLISION_EVENT_H
#define OBSTACLE_COLLISION_EVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"

class ObstacleCollisionEvent : public Event {
public:
    Entity obstacle;
    Entity target;
    ObstacleCollisionEvent(Entity obstacle, Entity target) : obstacle(obstacle), target(target) {}
};

#endif