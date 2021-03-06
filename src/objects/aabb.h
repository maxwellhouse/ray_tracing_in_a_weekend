#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#pragma warning(push, 0)
#include <glm/vec3.hpp>
#pragma warning(pop)

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
    aabb(const glm::vec3& min, const glm::vec3& max) : minVec(min), maxVec(max) {}
    glm::vec3 max() const { return maxVec; }
    glm::vec3 min() const { return minVec; }

    inline bool hit(const ray& r, float tMin, float tMax) const;

private:
    glm::vec3 minVec;
    glm::vec3 maxVec;
};

inline bool aabb::hit(const ray& r, float tMin, float tMax) const
{
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / r.direction()[a];
        float v = r.origin()[a];
        float t0 = (minVec[a] - v) * invD;
        float t1 = (maxVec[a] - v) * invD;
        if (invD < 0.0f)
        {
            std::swap(t0, t1);
        }
        tMin = ffmax(t0, tMin);
        tMax = ffmin(t1, tMax);
        if (tMax <= tMin)
        {
            return true;
        }
    }
    return false;
}

inline aabb surrounding_box(const aabb& box1, const aabb& box2)
{
    glm::vec3 small(ffmin(box1.min().x, box2.min().x), ffmin(box1.min().y, box2.min().y), ffmin(box1.min().z, box2.min().z));
    glm::vec3 big(ffmax(box1.min().x, box2.min().x), ffmax(box1.min().y, box2.min().y), ffmax(box1.min().z, box2.min().z));
    return aabb(big, small);
}
