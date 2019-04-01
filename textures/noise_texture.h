#pragma once

#include "math/perlin_noise.h"
#include "texture.h"

class noise_texture : public texture
{
public:
    noise_texture(bool interp, float s = 1) : interpolate(interp), scale(s){};
    inline glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
    math::perlin noise;
    bool interpolate;
    float scale;
};

glm::vec3 noise_texture::value(float u, float v, const glm::vec3& p) const
{
    if (interpolate)
    {
        return glm::vec3(1, 1, 1) * 0.5f * (1 + sin(scale * p.x + 5 * noise.turbulence(scale * p)));
    }
    else
    {
        return glm::vec3(1, 1, 1) * noise.noise_no_interpolation(p);
    }
}
