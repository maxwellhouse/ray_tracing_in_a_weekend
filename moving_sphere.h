#pragma once
#include "object.h"

class moving_sphere : public object
{
public:
    moving_sphere() {}
    moving_sphere(const vec3& cen0, const vec3& cen1, const float t0, const float t1, const float r, material* pMat) : center0(cen0), center1(cen1), time0(t0), time1(t1)
    {
        radius = r;
        pMaterial = pMat;
    };
    inline vec3 centerAtTime(const float time) const;
    inline bool bounding_box(const float t0, const float t1, aabb& box) const override;
    inline bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;

private:
    float time0, time1;
    vec3 center0, center1;
    vec3 center;
    float radius;
    material* pMaterial;
};

vec3 moving_sphere::centerAtTime(float time) const
{
    return center0 + ((time - time0) / (time1 - time0))* (center1 - center0);
}

bool moving_sphere::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = aabb(center - vec3(radius, radius, radius), center - vec3(radius, radius, radius));
    return true;
}

bool moving_sphere::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    bool hit = false;
    vec3 origin_center = r.origin() - centerAtTime(rec.t);
    float a = dot(r.direction(), r.direction());
    float b = dot(origin_center, r.direction());
    float c = dot(origin_center, origin_center) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        float temp = (-b - std::sqrtf(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - centerAtTime(rec.t)) / radius;
            rec.pMaterial = pMaterial;
            hit = true;
        }
        else
        {
            temp = (-b + std::sqrtf(discriminant) / a);
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - centerAtTime(rec.t)) / radius;
                rec.pMaterial = pMaterial;
                hit = true;
            }
        }
    }
    return hit;
}
