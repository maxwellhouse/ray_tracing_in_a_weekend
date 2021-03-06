#pragma once
#include "object.h"

class moving_sphere : public object
{
public:
    moving_sphere() {}
    moving_sphere(const glm::vec3& cen0, const glm::vec3& cen1, const float t0, const float t1, const float r, material* pMat)
        : center0(cen0), center1(cen1), time0(t0), time1(t1)
    {
        radius = r;
        pMaterial = pMat;
    };
    inline glm::vec3 centerAtTime(const float time) const;
    inline bool bounding_box(const float t0, const float t1, aabb& box) const override;
    inline bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;

private:
    glm::vec3 center0, center1;
    float time0, time1;
    glm::vec3 center;
    float radius;
    material* pMaterial;
};

glm::vec3 moving_sphere::centerAtTime(float time) const
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::bounding_box(const float t0, const float t1, aabb& box) const
{
    glm::vec3 center0AtTime = centerAtTime(t0);
    glm::vec3 center1AtTime = centerAtTime(t1);
    aabb box0(center0AtTime - glm::vec3(radius, radius, radius), center0AtTime - glm::vec3(radius, radius, radius));
    aabb box1(center1AtTime - glm::vec3(radius, radius, radius), center1AtTime - glm::vec3(radius, radius, radius));

    box = surrounding_box(box0, box1);

    return true;
}

bool moving_sphere::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    glm::vec3 oc = r.origin() - centerAtTime(r.time());
    
    float a = glm::dot<3, float, glm::qualifier::lowp>(r.direction(), r.direction());
    float b = glm::dot<3, float, glm::qualifier::lowp>(oc, r.direction());
    float c = glm::dot<3, float, glm::qualifier::lowp>(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - std::sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - centerAtTime(r.time())) / radius;
            rec.pMaterial = pMaterial;
            return true;
        }
        temp = (-b + std::sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - centerAtTime(r.time())) / radius;
            rec.pMaterial = pMaterial;
            return true;
        }
    }
    return false;
}
