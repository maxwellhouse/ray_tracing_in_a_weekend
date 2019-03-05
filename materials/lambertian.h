#pragma once
#include "material.h"
#include "../textures/texture.h"

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(math::rand(), math::rand(), math::rand()) - vec3(1.0f, 1.0f, 1.0f);
    } while (dot(p, p) >= 1.0);
    return p;
}

class Lambertian : public material
{
public:
    Lambertian(texture* tex): albedo(tex) { }
    bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, ray_in.time());
        attenuation = albedo->value(rec.u,rec.v,rec.p);
        return true;
    }

private:
    texture* albedo;
};
