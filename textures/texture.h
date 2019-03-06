#pragma once
#include "math/vec3.h"

class texture
{
public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};
