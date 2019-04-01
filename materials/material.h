#pragma once

#include "math/ray.h"
#include "objects/object.h"
#include "math/math.h"

glm::vec3 random_in_unit_sphere()
{
    glm::vec3 p;
    do
    {
        p = 2.0f * glm::vec3(math::rand(), math::rand(), math::rand()) - glm::vec3(1.0f, 1.0f, 1.0f);
    } while (glm::dot<3, float, glm::qualifier::lowp>(p, p) >= 1.0);
    return p;
}

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, glm::vec3& attenuation, ray& scatter) const = 0;
    virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const { return glm::vec3(0.0f, 0.0f, 0.0f); }
};
