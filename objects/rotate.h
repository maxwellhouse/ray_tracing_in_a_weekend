#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#include "glm/glm/vec3.hpp"
#include "glm/glm/gtx/rotate_vector.hpp"

#include "object.h"
#include "math/math.h"

class rotate : public object
{
    public:
        rotate(object* pObj, float x, float y, float z);
        bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
        bool bounding_box(const float t0, const float t1, aabb& box) const override;

    private:
        void rotate_y(float angle);

    private:
        object* pObject;
        float xDeg;
        float yDeg;
        float zDeg;
        aabb bbox;
        bool hasBox;
};

rotate::rotate(object* pObj, float x, float y, float z): pObject(pObj), xDeg(x), yDeg(y), zDeg(z), hasBox(false)
{
    rotate_y(y);
}

bool rotate::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    bool hit = false;
    glm::vec3 origin = glm::rotate(r.origin(), glm::radians(yDeg), glm::vec3(0, 1, 0));
    glm::vec3 direction = glm::rotate(r.direction(), glm::radians(yDeg), glm::vec3(0, 1, 0));
    ray rotated_r(origin, direction, r.time());
    if(pObject->hit(rotated_r, t_min, t_max, rec))
    {
        rec.p = glm::rotate(rec.p, glm::radians(yDeg), glm::vec3(0, 1, 0));
        rec.normal = glm::rotate(rec.normal, glm::radians(yDeg), glm::vec3(0, 1, 0));
        hit = true;
    }
    return hit;
}

bool rotate::bounding_box(const float t0, const float t1, aabb& box) const
{
    box = bbox;
    return true;
}

void rotate::rotate_y(float angle)
{
    float radians = (math::PI / 180.0f) * angle;
    float sin_theta = std::sin(radians);
    float cos_theta = std::cos(radians);
    hasBox = pObject->bounding_box(0.0f, 1.0f, bbox);
    glm::vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i*bbox.max().x + (1-i)*bbox.min().x;
                float y = j*bbox.max().y + (1-j)*bbox.min().y;
                float z = k*bbox.max().z + (1-k)*bbox.min().z;
                float newx = cos_theta*x + sin_theta*z;
                float newz = -sin_theta*x + cos_theta*z;
                glm::vec3 tester(newx, y, newz);
                for ( int c = 0; c < 3; c++ )
                {
                    if ( tester[c] > max[c] )
                        max[c] = tester[c];
                    if ( tester[c] < min[c] )
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = aabb(min, max);
}