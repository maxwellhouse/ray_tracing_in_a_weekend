#pragma once
#include "object.h"

class bvh_node: public object
{
public:
    bvh_node() {}
    bvh_node(const object** obj_list, const int n, const float t0, const float t1);
    inline bool bounding_box(const float t0, const float t1, aabb& box) const override;
    inline bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;

private:
    object* pLeft;
    object* pRight;
    aabb box;
};

bool bvh_node::bounding_box(const float t0, const float t1, aabb& b) const
{
    b = box;
    return true;
}

bool bvh_node::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    bool did_hit = false;
    if (box.hit(r, t_min, t_max))
    {
        hit_record left_rec;
        hit_record right_rec;

        bool hit_left = pLeft->hit(r, t_min, t_max, left_rec);
        bool hit_right = pLeft->hit(r, t_min, t_max, right_rec);

        if (hit_left && hit_right)
        {
            if (left_rec.t < right_rec.t)
            {
                rec = left_rec;
            }
            else
            {
                rec = right_rec;
            }
            did_hit = true;
        }
        else if (hit_left)
        {
            rec = left_rec;
            did_hit = true;
        }
        else if (hit_right)
        {
            rec = right_rec;
            did_hit = true;
        }
    }
    return did_hit;
}
