#pragma once
#include "glm/glm/vec3.hpp"

class texture
{
public:
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};
