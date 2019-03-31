#pragma once

#include "object.h"

class flip_normals : public object
{
    public:
    flip_normals(object* pObj) : pObject(pObj){}
    bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
    bool bounding_box(const float t0, const float t1, aabb& box) const override;

    private:
    object* pObject;
};

bool flip_normals::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    if(pObject->hit(r, t_min, t_max, rec))
    {
        rec.normal = -rec.normal;
        return true;
    }
    return false;
}

bool flip_normals::bounding_box(const float t0, const float t1, aabb& box) const
{
    return pObject->bounding_box(t0, t1, box);
}