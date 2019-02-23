#pragma once
#include "texture.h"

#include <cmath>

class checker_texture : public texture
{
public:
    checker_texture() {};
    checker_texture(texture* t0, texture* t1): pOdd(t0), pEven(t1) {}
    inline vec3 value(float u, float v, const vec3& p) const override;

private:
    texture* pOdd;
    texture* pEven;
};

vec3 checker_texture::value(float u, float v, const vec3 & p) const
{
    float sines = std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
    if (sines < 0)
    {
        return pOdd->value(u, v, p);
    }
    else
    {
        return pEven->value(u, v, p);
    }
}