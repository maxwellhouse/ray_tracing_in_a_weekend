#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#pragma warning(push, 0)
#include <glm/vec3.hpp>
#pragma warning(pop)

class texture
{
public:
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};
