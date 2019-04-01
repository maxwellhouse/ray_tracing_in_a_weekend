#pragma once
#include "texture.h"
#include "math/math.h"

class checker_texture : public texture
{
public:
    checker_texture(){};
    checker_texture(texture* t0, texture* t1) : pOdd(t0), pEven(t1) {}
    inline glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
    texture* pOdd;
    texture* pEven;
};

glm::vec3 checker_texture::value(float u, float v, const glm::vec3& p) const
{
    float sines = math::fastFloatSin(10 * p.x) * math::fastFloatSin(10 * p.y) * math::fastFloatSin(10 * p.z);
    if (sines < 0)
    {
        return pOdd->value(u, v, p);
    }
    else
    {
        return pEven->value(u, v, p);
    }
}
