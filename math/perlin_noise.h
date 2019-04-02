#pragma once

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#include "glm/glm/vec3.hpp"
#include "glm/glm/geometric.hpp"

#include "math.h"

namespace math
{
class perlin
{
public:
    glm::vec3 noise_no_interpolation(const glm::vec3& p) const;
    float noise_trilinear_interpolation(const glm::vec3& p) const;
    float turbulence(const glm::vec3& p, int depth = 7) const;

private:
    float perlin_interp(glm::vec3 c[2][2][2], float u, float v, float w) const;

    static glm::vec3* ranVec;
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
};

float perlin::turbulence(const glm::vec3& p, int depth) const
{
    float accum = 0;
    glm::vec3 temp_p = p;
    float weight = 1.0;
    for (int i = 0; i < depth; i++)
    {
        accum += weight * noise_trilinear_interpolation(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }
    return fabs(accum);
}

float perlin::noise_trilinear_interpolation(const glm::vec3& p) const
{
    float u = p.x - floor(p.x);
    float v = p.y - floor(p.y);
    float w = p.z - floor(p.z);
    int i = floor(p.x);
    int j = floor(p.y);
    int k = floor(p.z);
    glm::vec3 c[2][2][2];
    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = ranVec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
    return perlin_interp(c, u, v, w);
}

glm::vec3 perlin::noise_no_interpolation(const glm::vec3& p) const
{
    int i = static_cast<int>(4 * p.x) & 255;
    int j = static_cast<int>(4 * p.y) & 255;
    int k = static_cast<int>(4 * p.y) & 255;
    return ranVec[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
}

float perlin::perlin_interp(glm::vec3 c[2][2][2], float u, float v, float w) const
{
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                glm::vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                         dot(c[i][j][k], weight_v);
            }
    return accum;
}

static glm::vec3* perlin_generate()
{
    glm::vec3* p = new glm::vec3[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = glm::normalize(glm::vec3(-1 + 2 * math::rand(), -1 + 2 * math::rand(), -1 + 2 * math::rand()));
    }
    return p;
}

static void perlin_permute(int* p, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int target = static_cast<int>(math::rand() * (i + 1));
        int tmp = p[i];
        p[i] = target;
        p[target] = tmp;
    }
}

static int* perlin_generate_perm()
{
    int* p = new int[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = i;
    }
    perlin_permute(p, 256);
    return p;
}

glm::vec3* perlin::ranVec = perlin_generate();
int* perlin::perm_x = perlin_generate_perm();
int* perlin::perm_y = perlin_generate_perm();
int* perlin::perm_z = perlin_generate_perm();

} // namespace math
