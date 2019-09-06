#pragma once

#pragma warning(push, 0)
#include <glm/geometric.hpp>
#pragma warning(pop)

#include "materials/material.h"
#include "object.h"

class sphere : public object
{
public:
    sphere() {}
    sphere(const glm::vec3& cen, const float r, material* pMat) : center(cen), radius(r), pMaterial(pMat){};
    inline bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
    inline bool bounding_box(const float t0, const float t1, aabb& box) const override;
    static inline void get_sphere_uv(const glm::vec3& p, float& u, float& v)
    {
        float phi = atan2(p.z, p.x);
        float theta = asin(p.y);
        u = 1 - (phi + math::PI) / (2 * math::PI);
        v = (theta + math::PI / 2) / math::PI;
    }

private:
    glm::vec3 center;
    float radius;
    material* pMaterial;
};

bool sphere::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot<3, float, glm::qualifier::lowp>(r.direction(), r.direction());
    float b = glm::dot<3, float, glm::qualifier::lowp>(oc, r.direction());
    float c = glm::dot<3, float, glm::qualifier::lowp>(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - std::sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.pMaterial = pMaterial;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.pMaterial = pMaterial;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(const float, const float, aabb& box) const
{
    box = aabb(center - glm::vec3(radius, radius, radius), center + glm::vec3(radius, radius, radius));
    return true;
}
