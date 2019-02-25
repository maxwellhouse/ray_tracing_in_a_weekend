#pragma once
#include "math/vec3.h"
#include "math/math.h"

namespace math
{
    class perlin
    {
    public:
        float noise_no_interpolation(const vec3& p) const;
        float noise_trilinear_interpolation(const vec3& p) const;

    private:
        float trilinear_interpolate(float c[2][2][2], float u, float v, float w) const;

        static float *ranFloat;
        static int *perm_x;
        static int *perm_y;
        static int *perm_z;
    };

    float perlin::noise_trilinear_interpolation(const vec3& p) const
    {
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        float u = p.x() - i;
        float v = p.y() - j;
        float w = p.z() - k;
        u = u * u*(3 - 2 * u);
        v = v * v*(3 - 2 * v);
        w = w * w*(3 - 2 * w);
        float c[2][2][2];
        for(int di = 0; di < 2; di++)
            for(int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = ranFloat[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
                }

        return trilinear_interpolate(c,u,v,w);
    }

    float perlin::noise_no_interpolation(const vec3& p) const
    {
        int i = static_cast<int>(4 * p.x()) & 255;
        int j = static_cast<int>(4 * p.y()) & 255;
        int k = static_cast<int>(4 * p.y()) & 255;
        return ranFloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }

    float perlin::trilinear_interpolate(float c[2][2][2], float u, float v, float w) const
    {
        float accum = 0.0f;
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    accum += (i*u + (1 - i)*(1 - u))*
                        (j*v + (1 - j)*(1 - v))*
                        (k*w + (1 - k)*(1 - w))* c[i][j][k];
                }
        return accum;
    }

    static float* perlin_generate()
    {
        float * p = new float[256];
        for (int i = 0; i < 256; i++)
        {
            p[i] = math::rand();
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
        int *p = new int[256];
        for (int i = 0; i < 256; i++)
        {
            p[i] = i;
        }
        perlin_permute(p, 256);
        return p;
    }

    float* perlin::ranFloat = perlin_generate();
    int* perlin::perm_x = perlin_generate_perm();
    int* perlin::perm_y = perlin_generate_perm();
    int* perlin::perm_z = perlin_generate_perm();

}
