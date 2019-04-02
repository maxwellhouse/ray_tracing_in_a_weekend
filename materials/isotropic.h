#pragma once

#include "material.h"
#include "textures/texture.h"

class isotropic : public material
{
    public:
        isotropic(texture* pTex) : pAlbedo(pTex) {}

        bool scatter(const ray& ray_in, const hit_record& rec, glm::vec3& attenuation, ray& scatter) const override;

    private:
        texture* pAlbedo;
};

bool isotropic::scatter(const ray& ray_in, const hit_record& rec, glm::vec3& attenuation, ray& scatter) const 
{
    scatter = ray(rec.p, random_in_unit_sphere());
    attenuation = pAlbedo->value(rec.u, rec.v, rec.p);
    return true;
}