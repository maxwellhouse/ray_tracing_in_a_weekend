#pragma once

#include "ray.h"
#include "object.h"

#include <random>
std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const = 0;
};
