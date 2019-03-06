#pragma once
#include "../textures/texture.h"
#include "material.h"

class Lambertian : public material
{
public:
    Lambertian(texture* tex) : albedo(tex) {}
    bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, ray_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    texture* albedo;
};
