#pragma once
#pragma warning(push, 0)
#include <glm/geometric.hpp>
#pragma warning(pop)
#include "math/math.h"
#include "math/ray.h"

glm::vec3 random_in_unit_disk()
{
    glm::vec3 p;
    do
    {
        p = 2.0f * glm::vec3(math::rand(), math::rand(), 0) - glm::vec3(1, 1, 0);
    } while (glm::dot<3, float, glm::qualifier::lowp>(p, p) >= 1.0f);
    return p;
}

class camera
{
public:
    // vfov is top to bottom in degrees
    camera(
        const glm::vec3& lookfrom,
        const glm::vec3& lookat,
        const glm::vec3& vup,
        const float vfov,
        const float aspect,
        const float aperture,
        const float focus_dist,
        const float t0,
        const float t1)
    {
        time0 = t0;
        time1 = t1;
        lens_radius = aperture / 2.0f;
        float theta = vfov * float(math::PI) / 180.0f;
        float half_height = tan(theta / 2.0f);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(glm::cross(vup, w));
        v = glm::cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2.0f * half_width * focus_dist * u;
        vertical = 2.0f * half_height * focus_dist * v;
    }
    ray get_ray(float s, float t)
    {
        glm::vec3 rd = lens_radius * random_in_unit_disk();
        glm::vec3 offset = u * rd.x + v * rd.y;
        float time = time0 + math::rand() * (time1 - time0);
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
    }

    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
    float lens_radius;
    float time0, time1;
};
