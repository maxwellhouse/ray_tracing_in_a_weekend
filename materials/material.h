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
};
