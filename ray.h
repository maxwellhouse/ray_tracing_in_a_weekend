#pragma once
#include "vec3.h"

class ray
{
public:
    ray() {};
    ray(const vec3& a, const vec3& b, const float ti = 0.0f) { A = a; B = b; t = ti; }

    inline vec3 origin() const { return A; }
    inline vec3 direction() const { return B; }
    inline vec3 point_at_parameter(const float t) const { return A + t * B; }
    inline float time() const { return t; }

private:
    vec3 A;
    vec3 B;
    float t;
};
