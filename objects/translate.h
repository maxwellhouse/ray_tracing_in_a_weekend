#pragma once

#include "object.h"

class translate : public object
{
    public:
        translate(object* pObj, const vec3& displacement): pObject(pObj), offset(displacement) {}
        bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
        bool bounding_box(const float t0, const float t1, aabb& box) const override;

    private:
    object* pObject;
    vec3 offset;
};

bool translate::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    bool success = false;
    ray moved_ray(r.origin() - offset, r.direction(), r.time());
    if(pObject->hit(moved_ray, t_min, t_max, rec))
    {
        rec.p += offset;
        success = true;
    }
    return success;
}

bool translate::bounding_box(const float t0, const float t1, aabb& box) const 
{
    bool success = false;
    if(pObject->bounding_box(t0, t1, box))
    {
        box = aabb(box.min() + offset, box.max() + offset);
        success = true;
    }
    return success;
}