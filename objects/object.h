#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-include"
#include "math/vec3.h"
#include "math/ray.h"
#pragma clang diagnostic pop
#include "aabb.h"

class material;

struct hit_record
{
    float t;
    float u;
    float v;
    vec3 p;
    vec3 normal;
    material* pMaterial;
};

class object
{
public:
    virtual bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(const float t0, const float t1, aabb& box) const = 0;
};

inline bool surrounding_box(const std::vector<object*> obj_list, const float t0, const float t1, aabb& box)
{
    if (obj_list.empty() == false)
    {
        aabb temp_box;
        for (const auto& obj : obj_list)
        {
            if (obj->bounding_box(t0, t1, temp_box))
            {
                box = surrounding_box(box, temp_box);
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}
