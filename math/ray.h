#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#include "glm/glm/vec3.hpp"

class ray
{
public:
    ray(){};
    ray(const glm::vec3& a, const glm::vec3& b, const float ti = 0.0f)
    {
        A = a;
        B = b;
        t = ti;
    }

    inline glm::vec3 origin() const { return A; }
    inline glm::vec3 direction() const { return B; }
    inline glm::vec3 point_at_parameter(const float t) const { return A + t * B; }
    inline float time() const { return t; }

private:
    glm::vec3 A;
    glm::vec3 B;
    float t;
};
