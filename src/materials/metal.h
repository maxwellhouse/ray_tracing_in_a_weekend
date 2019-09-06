#pragma once
#pragma warning(push, 0)
#include <glm/geometric.hpp>
#pragma warning(pop)

#include "material.h"

glm::vec3 reflect(const glm::vec3& v1, const glm::vec3& v2)
{
    return v1 - 2 * glm::dot<3, float, glm::qualifier::lowp>(v1, v2) * v2;
}

class metal : public material
{
public:
    metal(const glm::vec3& v, const float f) : albedo(v)
    {
        if (f < 1.0f)
            fuzz = f;
        else
            fuzz = 1;
    }

    bool scatter(const ray& ray_in, const hit_record& rec, glm::vec3& attenuation, ray& scatter) const override
    {
        glm::vec3 reflected = reflect(glm::normalize(ray_in.direction()), rec.normal);
        scatter = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
        attenuation = albedo;
        
        return (glm::dot<3, float, glm::qualifier::lowp>(scatter.direction(), rec.normal) > 0);
    }

private:
    glm::vec3 albedo;
    float fuzz;
};
