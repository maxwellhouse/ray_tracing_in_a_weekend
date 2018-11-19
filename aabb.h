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

bool aabb::hit(const ray& r, const float tMin, const float tMax) const
{
    for (int a = 0; a < 3; a++)
    {
        float t0 = ffmin((minVec[a] - r.origin()[a]) / r.direction()[a],
            (maxVec[a] - r.origin()[a]) / r.direction()[a]);
        float t1 = ffmax((minVec[a] - r.origin()[a]) / r.direction()[a],
            (maxVec[a] - r.origin()[a]) / r.direction()[a]);

        float min = ffmax(t0, tMin);
        float max = ffmin(t1, tMax);
        if (tMax <= tMin)
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
