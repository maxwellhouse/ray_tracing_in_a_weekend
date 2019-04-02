#pragma once

#include "object.h"
#include "object_list.h"
#include "flip_normals.h"
#include "rectangle.h"

class box : public object
{
    public:
        box(const glm::vec3& p0, const glm::vec3& p1, material* pMat);
        bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
        bool bounding_box(const float t0, const float t1, aabb& box) const override;

    private:
        glm::vec3 pMin;
        glm::vec3 pMax;
        material* pMaterial;
        object_list* pList;
};

box::box(const glm::vec3& p0, const glm::vec3& p1, material* pMat)
{
    int num_objects = 6;
    pMin = p0;
    pMax = p1;
    auto pObjects = new object*[num_objects];
    pObjects[0] = new xy_rectangle(p0.x, p1.x, p0.y, p1.y, p1.z, pMat);
    pObjects[1] = new flip_normals(new xy_rectangle(p0.x, p1.x, p0.y, p1.y, p0.z, pMat));
    pObjects[2] = new xz_rectangle(p0.x, p1.x, p0.z, p1.z, p1.y, pMat);
    pObjects[3] = new flip_normals(new xz_rectangle(p0.x, p1.x, p0.z, p1.z, p0.y, pMat));
    pObjects[4] = new yz_rectangle(p0.y, p1.y, p0.z, p1.z, p1.x, pMat);
    pObjects[5] = new flip_normals(new yz_rectangle(p0.y, p1.y, p0.z, p1.z, p0.x, pMat));

    pList = new object_list(pObjects, num_objects);
}

bool box::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    return pList->hit(r, t_min, t_max, rec);
}

bool box::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = aabb(pMin, pMax);
    return true;
}