#pragma once


#include "material.h"

#include <random>

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(distribution(generator), distribution(generator), distribution(generator)) - vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0);
    return p;
}

class Lambertian : public material
{
public:
    Lambertian(const vec3& v): albedo(v) { }
    bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, ray_in.time());
        attenuation = albedo;
        return true;
    }

private:
    vec3 albedo;
};
