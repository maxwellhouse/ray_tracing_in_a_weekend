#pragma once

#include "material.h"

vec3 reflect(const vec3& v1, const vec3& v2)
{
    return v1 - 2 * dot(v1, v2) * v2;
}

class metal : public material
{
public:
    metal(const vec3& v, const float f) : albedo(v) { if (f < 1.0f) fuzz = f; else fuzz = 1; }

    bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const override
    {
        vec3 reflected = reflect(make_unit_vector(ray_in.direction()), rec.normal);
        scatter = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
        attenuation = albedo;
        return (dot(scatter.direction(), rec.normal) > 0);
    }

private:
    vec3 albedo;
    float fuzz;
};
