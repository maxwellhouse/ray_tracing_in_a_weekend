#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#pragma warning(push, 0)
#include <glm/vec3.hpp>
#pragma warning(pop)

#include "math/ray.h"
#include "aabb.h"

class material;

struct hit_record
{
    float t;
    float u;
    float v;
    glm::vec3 p;
    glm::vec3 normal;
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
