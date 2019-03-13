#pragma once
#include "materials/material.h"
#include "object.h"

class xy_rectangle : public object
{
public:
    xy_rectangle(float _x0, float _x1, float _y0, float _y1, float _k, material* pMat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), m_pMaterial(pMat)
    {
    }
    xy_rectangle() {}
    bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
    bool bounding_box(const float t0, const float t1, aabb& box) const override;

private:
    float x0;
    float x1;
    float y0;
    float y1;
    float k;
    material* m_pMaterial;
};

bool xy_rectangle::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    float t = (k - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
    {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.pMaterial = m_pMaterial;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0.0f, 0.0f, 1.0f);
    return true;
}

inline bool xy_rectangle::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = aabb(vec3(x0, y0, k - 0.001f), vec3(x1, y1, k + 0.001f));
    return true;
}

class xz_rectangle : public object
{
public:
    xz_rectangle(float _x0, float _x1, float _z0, float _z1, float _k, material* pMat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), m_pMaterial(pMat)
    {
    }
    xz_rectangle() {}
    bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
    bool bounding_box(const float t0, const float t1, aabb& box) const override;

private:
    float x0;
    float x1;
    float z0;
    float z1;
    float k;
    material* m_pMaterial;
};

bool xz_rectangle::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    float t = (k - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    float x = r.origin().x() + t * r.direction().x();
    float z = r.origin().z() + t * r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
    {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.pMaterial = m_pMaterial;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0.0f, 1.0f, 0.0f);
    return true;
}

bool xz_rectangle::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = aabb(vec3(x0, k - 0.0001f, z0), vec3(x1, k + 0.0001f, z1));
    return true;
}

class yz_rectangle : public object
{
public:
    yz_rectangle(float _y0, float _y1, float _z0, float _z1, float _k, material* pMat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), m_pMaterial(pMat)
    {
    }
    yz_rectangle() {}
    bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
    bool bounding_box(const float t0, const float t1, aabb& box) const override;

private:
    float y0;
    float y1;
    float z0;
    float z1;
    float k;
    material* m_pMaterial;
};

bool yz_rectangle::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    float t = (k - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    float y = r.origin().y() + t * r.direction().y();
    float z = r.origin().z() + t * r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
    {
        return false;
    }

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.pMaterial = m_pMaterial;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(1.0f, 0.0f, 0.0f);
    return true;
}

bool yz_rectangle::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = aabb(vec3(k - 0.0001f, y0, z0), vec3(k + 0.0001f, y1, z1));
    return true;
}
