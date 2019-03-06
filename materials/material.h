#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-include"
#include "math/ray.h"
#pragma clang diagnostic pop
#include "objects/object.h"

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const = 0;
    virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0.0f, 0.0f, 0.0f); }
};
