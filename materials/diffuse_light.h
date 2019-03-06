#pragma once

#include "material.h"
#include "textures/texture.h"

class diffuse_light : public material
{
public:
    diffuse_light(texture* pTexture) : pEmit(pTexture) {}
    bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const override;
    vec3 emitted(float u, float v, const vec3& p) const override;

private:
    texture* pEmit;
};

bool diffuse_light::scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scatter) const
{
    return false;
}

inline vec3 diffuse_light::emitted(float u, float v, const vec3& p) const
{
    return pEmit->value(u, v, p);
}
