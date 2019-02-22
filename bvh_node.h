#pragma once
#include "object.h"
#include "math.h"

#include <iostream>

class bvh_node: public object
{
public:
    bvh_node() {}
    bvh_node(object** obj_list, const int n, const float t0, const float t1);
    inline bool bounding_box(const float t0, const float t1, aabb& box) const override;
    inline bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;

private:
    object* pLeft;
    object* pRight;
    aabb box;
};


inline int box_x_compare(const void* a, const void* b)
{
    aabb box_left;
    aabb box_right;
    object *ah = *(object**)a;
    object* bh = *(object**)b;
    if (ah->bounding_box(0, 0, box_left) == false || bh->bounding_box(0, 0, box_right) == false)
    {
        std::cerr << "No bounding box in bvh node constructor.\n";
    }
    if (box_left.min().x() - box_right.min().x() < 0.0f)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

inline int box_y_compare(const void* a, const void* b)
{
    aabb box_left;
    aabb box_right;
    object *ah = *(object**)a;
    object* bh = *(object**)b;
    if (ah->bounding_box(0, 0, box_left) == false || bh->bounding_box(0, 0, box_right) == false)
    {
        std::cerr << "No bounding box in bvh node constructor.\n";
    }
    if (box_left.min().y() - box_right.min().y() < 0.0f)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

inline int box_z_compare(const void* a, const void* b)
{
    aabb box_left;
    aabb box_right;
    object *ah = *(object**)a;
    object* bh = *(object**)b;
    if (ah->bounding_box(0, 0, box_left) == false || bh->bounding_box(0, 0, box_right) == false)
    {
        std::cerr << "No bounding box in bvh node constructor.\n";
    }
    if (box_left.min().z() - box_right.min().z() < 0.0f)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

inline bvh_node::bvh_node(object ** obj_list, const int n, const float t0, const float t1)
{
    aabb box_left;
    aabb box_right;
    int axis = int(3 * math::distribution(math::random_number));
    if (axis == 0)
    {
        qsort(obj_list, n, sizeof(object*), box_x_compare);
    }
    else if (axis == 1)
    {
        qsort(obj_list, n, sizeof(object*), box_y_compare);
    }
    else
    {
        qsort(obj_list, n, sizeof(object*), box_z_compare);
    }

    if (n == 1)
    {
        pLeft = pRight = obj_list[0];
    }
    else if (n == 2)
    {
        pLeft = obj_list[0];
        pRight = obj_list[1];
    }
    else
    {
        pLeft = new bvh_node(obj_list, n / 2, t0, t1);
        pRight = new bvh_node(obj_list + n / 2, n - (n / 2), t0, t1);
    }
    if (pLeft->bounding_box(t0, t1, box_left) == false || pRight->bounding_box(t0, t1, box_right) == false)
    {
        std::cerr << "No bounding box in bvh node constructor.\n";
    }
    else
    {
        box = surrounding_box(box_left, box_right);
    }
}

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
