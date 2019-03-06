#pragma once

#include "texture.h"

class constant_texture : public texture
{
public:
    constant_texture() {}
    constant_texture(const vec3& c) : color(c) {}
    vec3 value(float u, float v, const vec3& p) const override;

private:
    vec3 color;
};

vec3 constant_texture::value(float u, float v, const vec3& p) const
{
    return color;
}
