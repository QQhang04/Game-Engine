#ifndef TRANSFORM_COMPONENTS_H
#define TRANSFORM_COMPONENTS_H

#include <glm/glm.hpp>

class TransformComponents {
    public:
        glm::vec2 position;
        glm::vec2 scale;
        double rotation;
};
#endif