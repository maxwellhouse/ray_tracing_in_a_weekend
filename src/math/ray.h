#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#pragma warning(push, 0)
#include <glm/vec3.hpp>
#pragma warning(pop)

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
    inline glm::vec3 point_at_parameter(const float time) const { return A + time * B; }
    inline float time() const { return t; }

private:
    glm::vec3 A;
    glm::vec3 B;
    float t;
};
