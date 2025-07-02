#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include <glm/glm.hpp>

struct BoxColliderComponent
{
    glm::vec2 size = glm::vec2(0.0f, 0.0f);
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);

    BoxColliderComponent(glm::vec2 size = glm::vec2(0.0f, 0.0f), glm::vec2 offset = glm::vec2(0.0f, 0.0f)) : size(size), offset(offset) {}
};
#endif