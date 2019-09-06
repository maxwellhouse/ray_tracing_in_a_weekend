#pragma once
#include "texture.h"

class image_texture : public texture
{
public:
    image_texture() {}
    image_texture(unsigned char* pixels, int width, int height) : data(pixels), nx(width), ny(height) {}
    inline glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
    unsigned char* data;
    int nx;
    int ny;
};

glm::vec3 image_texture::value(float u, float v, const glm::vec3&) const
{
    int i = static_cast<int>((u)*nx);
    int j = static_cast<int>((1 - v) * ny - 0.001f);
    if (i < 0)
        i = 0;
    if (j < 0)
        j = 0;
    if (i > nx - 1)
        i = nx - 1;
    if (j > ny - 1)
        j = ny - 1;
    float r = int(data[3 * i + 3 * nx * j]) / 255.0f;
    float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0f;
    float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0f;
    return glm::vec3(r, g, b);
}
