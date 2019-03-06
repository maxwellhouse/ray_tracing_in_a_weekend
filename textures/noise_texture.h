#pragma once

#include "math/perlin_noise.h"
#include "texture.h"

class noise_texture : public texture
{
public:
    noise_texture(bool interp, float s = 1) : interpolate(interp), scale(s){};
    inline vec3 value(float u, float v, const vec3& p) const override;

private:
    math::perlin noise;
    bool interpolate;
    float scale;
};

vec3 noise_texture::value(float u, float v, const vec3& p) const
{
    if (interpolate)
    {
        return vec3(1, 1, 1) * 0.5 * (1 + sin(scale * p.x() + 5 * noise.turbulence(scale * p)));
    }
    else
    {
        return vec3(1, 1, 1) * noise.noise_no_interpolation(p);
    }
}
