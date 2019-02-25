#pragma once

#include "math/ray.h"
#include "objects/object.h"

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const = 0;
};
