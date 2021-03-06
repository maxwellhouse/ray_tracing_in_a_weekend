#pragma once

#include "material.h"
#include "metal.h"

bool refract(const glm::vec3& v, const glm::vec3& normal, float ni_over_nt, glm::vec3& refracted)
{
    bool ret = false;
    glm::vec3 uv = glm::normalize(v);
    float dt = dot(uv, normal);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0.0f)
    {
        refracted = ni_over_nt * (uv - normal * dt) - normal * std::sqrt(discriminant);
        ret = true;
    }
    return ret;
}

float schlick(const float cosine, const float reflective_index)
{
    float r0 = (1 - reflective_index) / (1 + reflective_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * powf((1 - cosine), 5);
}

class dielectric : public material
{
public:
    dielectric(const float i) : reflective_index(i) {}
    bool scatter(const ray& ray_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const override
    {
        glm::vec3 outward_normal;
        glm::vec3 reflected = reflect(ray_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
        float reflect_prob = 1.0f;
        float cosine;
        glm::vec3 refracted;

        float dot = glm::dot<3, float, glm::qualifier::lowp>(ray_in.direction(), rec.normal);
        if (dot > 0.0f)
        {
            outward_normal = -rec.normal;
            ni_over_nt = reflective_index;
            cosine = reflective_index * dot / ray_in.direction().length();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / reflective_index;
            cosine = -dot / ray_in.direction().length();
        }

        if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = schlick(cosine, reflective_index);
        }

        if (math::rand() < reflect_prob)
        {
            scattered = ray(rec.p, reflected, ray_in.time());
        }
        else
        {
            scattered = ray(rec.p, refracted, ray_in.time());
        }

        return true;
    }

private:
    float reflective_index;
};
