#pragma once

#include "math/ray.h"
#include "objects/object.h"

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(math::rand(), math::rand(), math::rand()) - vec3(1.0f, 1.0f, 1.0f);
    } while (dot(p, p) >= 1.0);
    return p;
}

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const = 0;
    virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0.0f, 0.0f, 0.0f); }
};
