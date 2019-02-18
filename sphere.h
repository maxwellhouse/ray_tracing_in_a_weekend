#pragma once

#include "object.h"
#include "material.h"

class sphere : public object
{
public:
    sphere() {}
    sphere(const vec3& cen, const float r, material* pMat) : center(cen), radius(r), pMaterial(pMat) {};
    inline bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
    inline bool bounding_box(const float t0, const float t1, aabb& box) const override;

private:
    vec3 center;
    float radius;
    material* pMaterial;
};

bool sphere::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    bool hit = false;
    vec3 origin_center = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(origin_center, r.direction());
    float c = dot(origin_center, origin_center) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        float temp = (-b - sqrtf(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.pMaterial = pMaterial;
            hit = true;
        }
        else
        {
            temp = (-b + sqrtf(discriminant) / a);
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.pMaterial = pMaterial;
                hit = true;
            }
        }
    }
    return hit;
}

bool sphere::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = aabb(center - vec3(radius, radius, radius), center - vec3(radius, radius, radius));
    return true;
}
