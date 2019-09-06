#pragma once

#include "texture.h"

class constant_texture : public texture
{
public:
    constant_texture() {}
    constant_texture(const glm::vec3& c) : color(c) {}
    glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
    glm::vec3 color;
};

glm::vec3 constant_texture::value(float, float, const glm::vec3&) const
{
    return color;
}
