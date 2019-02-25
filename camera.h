#pragma once

#include <cmath>

#include "math/ray.h"
#include "math/math.h"

vec3 random_in_unit_disk() 
{
    vec3 p;
    do {
        p = 2.0 * vec3(math::distribution(math::random_number), math::distribution(math::random_number), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
public:
    // vfov is top to bottom in degrees
    camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, const float vfov, const float aspect, const float aperture, const float focus_dist, const float t0, const float t1) 
    {
        time0 = t0;
        time1 = t1;
        lens_radius = aperture / 2;
        float theta = vfov * float(math::PI) / 180.0f;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = make_unit_vector(lookfrom - lookat);
        u = make_unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist*u - half_height * focus_dist*v - focus_dist * w;
        horizontal = 2 * half_width*focus_dist*u;
        vertical = 2 * half_height*focus_dist*v;
    }
    ray get_ray(float s, float t) 
    {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        float time = time0 + math::distribution(math::random_number)*(time1 - time0);
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
    float time0, time1;
};
