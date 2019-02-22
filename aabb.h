#pragma once
#include <vector>

inline float ffmin(float a, float b)
{
    return a < b ? a : b;
}

inline float ffmax(float a, float b)
{
    return a > b ? a : b;
}

class aabb
{
public:
    aabb() {}
    aabb(const vec3& min, const vec3& max) : minVec(min), maxVec(max) {}
    vec3 max() const { return maxVec; }
    vec3 min() const { return minVec; }

    inline bool hit(const ray& r, const float tMin, const float tMax) const;

private:
    vec3 minVec;
    vec3 maxVec;
};

inline bool aabb::hit(const ray& r, const float tMin, const float tMax) const
{
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / r.direction()[a];
        float t0 = (min()[a] - r.origin()[a]) * invD;
        float t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
        {
            std::swap(t0, t1);
        }
        float tmin = t0 > tmin ? t0 : tmin;
        float tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
        {
            return true;
        }
    }
    return false;
}

inline aabb surrounding_box(const aabb& box1, const aabb& box2)
{
    vec3 small(ffmin(box1.min().x(), box2.min().x()), ffmin(box1.min().y(), box2.min().y()), ffmin(box1.min().z(), box2.min().z()));
    vec3 big(ffmax(box1.min().x(), box2.min().x()), ffmax(box1.min().y(), box2.min().y()), ffmax(box1.min().z(), box2.min().z()));
    return aabb(small, big);
}
