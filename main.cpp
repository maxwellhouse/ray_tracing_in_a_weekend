#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb/stb_image.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-include"
#include "camera.h"
#include "materials/dielectric.h"
#include "materials/diffuse_light.h"
#include "materials/lambertian.h"
#include "materials/material.h"
#include "materials/metal.h"
#include "math/ray.h"
#include "math/vec3.h"
#include "objects/bvh_node.h"
#include "objects/moving_sphere.h"
#include "objects/object_list.h"
#include "objects/rectangle.h"
#include "objects/sphere.h"
#include "textures/checker_texture.h"
#include "textures/constant_texture.h"
#include "textures/image_texture.h"
#include "textures/noise_texture.h"
#pragma clang diagnostic pop

#include <chrono>

#include <iostream>

vec3 linear_interp_color(const ray& r, const object* obj, const int depth)
{
    hit_record rec;
    if (obj->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
    {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.pMaterial->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.pMaterial->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * linear_interp_color(scattered, obj, depth + 1);
        }
        else
        {
            return emitted;
        }
    }

    return vec3(0, 0, 0);
}

object* make_random_world()
{
    int i = 0;
    int num_objects = 11;
    auto pList = new object*[482];
    pList[i++] = new sphere(vec3(0, -1000, 0), 1000, new Lambertian(new constant_texture(vec3(0.5f, 0.5f, 0.5f))));

    for (int a = -num_objects; a < num_objects; a++)
    {
        for (int b = -num_objects; b < num_objects; b++)
        {
            float choose_material = math::rand();
            vec3 center(a + 0.9f * math::rand(), 0.2f, b + 0.9f * math::rand());
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_material < 0.8f)
                { // diffuse
                    if (math::random_number() % 2)
                    {
                        pList[i++] = new sphere(
                            center,
                            0.2f,
                            new Lambertian(new constant_texture(
                                vec3(math::rand() * math::rand(), math::rand() * math::rand(), math::rand() * math::rand()))));
                    }
                    else
                    {
                        pList[i++] = new moving_sphere(
                            center,
                            center + vec3(0.0f, 0.5f * math::rand(), 0.0f),
                            0.0f,
                            1.0f,
                            0.2f,
                            new Lambertian(new constant_texture(
                                vec3(math::rand() * math::rand(), math::rand() * math::rand(), math::rand() * math::rand()))));
                    }
                }
                else if (choose_material < 0.95f)
                { // metal
                    pList[i++] = new sphere(
                        center,
                        0.2f,
                        new metal(
                            vec3(0.5f * (1 + math::rand()), 0.5f * (1 + math::rand()), 0.5f * (1 + math::rand())), 0.5f * math::rand()));
                }
                else
                { // glass
                    pList[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
                }
            }
        }
    }
    return new bvh_node(pList, i, 0.0f, 1.0f);
}

object* make_test_world()
{
    int num_objects = 5;
    auto pList = new object*[num_objects];
    pList[0] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(new constant_texture(vec3(0.4f, 0.2f, 0.1f))));
    pList[1] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
    pList[2] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));
    vec3 center(0.0f, -4.0f, 0.0f);
    pList[3] = new moving_sphere(
        center,
        center + vec3(0.0f, 0.5f * math::rand(), 0.0f),
        0.0f,
        0.75f,
        1.0f,
        new Lambertian(new constant_texture(vec3(math::rand() * math::rand(), math::rand() * math::rand(), math::rand() * math::rand()))));

    auto pBlack = new constant_texture(vec3(0.0f, 0.0f, 0.0f));
    auto pWhite = new constant_texture(vec3(1.0f, 1.0f, 1.0f));
    auto pChecker = new checker_texture(pBlack, pWhite);
    pList[4] = new sphere(vec3(0, -1000, 0), 1000, new Lambertian(pChecker));

    //return new bvh_node(pList, num_objects, 0.0f, 1.0f);
    return new object_list(pList, num_objects);
}

object* make_two_perlin_spheres()
{
    int num_objects = 2;
    auto pList = new object*[num_objects];
    auto pNoise = new noise_texture(true, 1.0f);
    pList[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000, new Lambertian(pNoise));
    pList[1] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2, new Lambertian(pNoise));

    return new bvh_node(pList, num_objects, 0.0f, 1.0f);
}

object* make_two_spheres()
{
    int num_objects = 2;
    auto pList = new object*[num_objects];
    auto pBlue = new constant_texture(vec3(0.0f, 0.0f, 0.6f));
    auto pGreen = new constant_texture(vec3(0.0f, 0.6f, 0.0f));
    auto pChecker = new checker_texture(pBlue, pGreen);
    auto pRevChecker = new checker_texture(pGreen, pBlue);
    pList[0] = new sphere(vec3(0.0f, -10.0f, 0.0f), 10, new Lambertian(pChecker));
    pList[1] = new sphere(vec3(0.0f, 10.0f, 0.0f), 10, new Lambertian(pRevChecker));

    return new bvh_node(pList, num_objects, 0.0f, 1.0f);
}

object* make_textured_sphere()
{
    int num_objects = 2;
    auto pList = new object*[num_objects];
    int nx, ny, nn;
    unsigned char* tex_data =
        stbi_load("E:/Projects/ray_tracing_in_a_weekend/physical_earth_satellite_image_mural_lg.jpg", &nx, &ny, &nn, 0);
    auto pImageTexture = new image_texture(tex_data, nx, ny);
    pList[0] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2, new Lambertian(pImageTexture));
    auto pNoise = new noise_texture(true, 1.0f);
    pList[1] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000, new Lambertian(pNoise));
    return new bvh_node(pList, num_objects, 0.0f, 1.0f);
}

object* make_simple_light()
{
    int num_objects = 4;
    auto pList = new object*[num_objects];
    auto pPerlinTexture = new noise_texture(true, 4.0f);
    pList[0] = new sphere(vec3(0, -1000, 0), 1000, new Lambertian(pPerlinTexture));
    pList[1] = new sphere(vec3(0, 2, 0), 2, new Lambertian(pPerlinTexture));
    pList[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    pList[3] = new xy_rectangle(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    return new object_list(pList, num_objects);
}

object* make_cornell_box()
{
    int num_objects = 5;
    auto pList = new object*[num_objects];
    material* pRed = new Lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
    material* pWhite = new Lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
    material* pGreen = new Lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
    material* pLight = new diffuse_light(new constant_texture(vec3(15.0f, 15.0f, 15.0f)));
    pList[0] = new yz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, pGreen);
    pList[1] = new yz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, pRed);
    pList[2] = new xz_rectangle(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, pLight);
    pList[3] = new xz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, pWhite);
    pList[4] = new xy_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, pWhite);

    return new object_list(pList, num_objects);
}

int main()
{
    const int width = 800;
    const int height = 800;
    const float num_samples = 100.0f;
    int comp = 3;
    std::vector<uint8_t> image;
    const int bitsPerPixel = comp * (8 * sizeof(uint8_t));
    int stride = width * bitsPerPixel; // bits per row
    stride += 31;                      // round up to next 32-bit boundary
    stride /= 32;                      // DWORDs per row
    stride *= 4;                       // bytes per row

    //object* obj_list = make_test_world();
    //object* obj_list = make_random_world();
    //object* obj_list = make_textured_sphere();
    //object* obj_list = make_simple_light();
    object* obj_list = make_cornell_box();

    //vec3 lookfrom(0.0f, 2.0f, 10.0f);
    //vec3 lookat(0.0f, 0.0f, 0.0f);
    vec3 lookfrom(278.0f, 278.0f, -800.0f);
    vec3 lookat(278.0f, 278.0f, 0.0f);
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    float fov = 40.0f;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), fov, float(width) / float(height), aperture, dist_to_focus, 0.0, 1.0);

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            vec3 color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < num_samples; s++)
            {
                float u = float(i + math::rand()) / float(width);
                float v = float(j + math::rand()) / float(height);
                ray r = cam.get_ray(u, v);

                color += linear_interp_color(r, obj_list, 0);
            }
            color /= num_samples;
            color = vec3(std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]));
            auto ir = uint8_t(255.99f * color.r());
            auto ig = uint8_t(255.99f * color.g());
            auto ib = uint8_t(255.99f * color.b());
            image.push_back(ir);
            image.push_back(ig);
            image.push_back(ib);
        }
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << time_span.count() << " seconds.";

    stbi_write_png("test.png", width, height, comp, image.data(), stride);
}
