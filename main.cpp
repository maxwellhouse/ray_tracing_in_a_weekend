#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
//#define GLM_FORCE_MESSAGES
#include "glm/glm/vec3.hpp"

#include "camera.h"
#include "materials/dielectric.h"
#include "materials/diffuse_light.h"
#include "materials/lambertian.h"
#include "materials/material.h"
#include "materials/metal.h"
#include "math/ray.h"
#include "objects/bvh_node.h"
#include "objects/moving_sphere.h"
#include "objects/object_list.h"
#include "objects/rectangle.h"
#include "objects/sphere.h"
#include "objects/flip_normals.h"
#include "objects/box.h"
#include "objects/rotate.h"
#include "objects/translate.h"
#include "objects/constant_medium.h"
#include "textures/checker_texture.h"
#include "textures/constant_texture.h"
#include "textures/image_texture.h"
#include "textures/noise_texture.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>
#include <future>

glm::vec3 linear_interp_color(const ray& r, const object* obj, const int depth)
{
    hit_record rec;
    if (obj->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
    {
        ray scattered;
        glm::vec3 attenuation;
        glm::vec3 emitted = rec.pMaterial->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.pMaterial->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * linear_interp_color(scattered, obj, depth + 1);
        }
        else
        {
            return emitted;
        }
    }

    return glm::vec3(0, 0, 0);
}

object* make_random_world()
{
    int i = 0;
    int num_objects = 11;
    material* pLight = new diffuse_light(new constant_texture(glm::vec3(15.0f, 15.0f, 15.0f)));
    auto pList = new object*[482];
    pList[i++] = new sphere(glm::vec3(0, -1000, 0), 1000, pLight);

    for (int a = -num_objects; a < num_objects; a++)
    {
        for (int b = -num_objects; b < num_objects; b++)
        {
            float choose_material = math::rand();
            glm::vec3 center(a + 0.9f * math::rand(), 0.2f, b + 0.9f * math::rand());
            if ((center - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_material < 0.8f)
                { // diffuse
                    if (math::random_number() % 2)
                    {
                        pList[i++] = new sphere(
                            center,
                            0.2f,
                            new Lambertian(new constant_texture(
                                glm::vec3(math::rand() * math::rand(), math::rand() * math::rand(), math::rand() * math::rand()))));
                    }
                    else
                    {
                        pList[i++] = new moving_sphere(
                            center,
                            center + glm::vec3(0.0f, 0.5f * math::rand(), 0.0f),
                            0.0f,
                            1.0f,
                            0.2f,
                            new Lambertian(new constant_texture(
                                glm::vec3(math::rand() * math::rand(), math::rand() * math::rand(), math::rand() * math::rand()))));
                    }
                }
                else if (choose_material < 0.95f)
                { // metal
                    pList[i++] = new sphere(
                        center,
                        0.2f,
                        new metal(
                            glm::vec3(0.5f * (1 + math::rand()), 0.5f * (1 + math::rand()), 0.5f * (1 + math::rand())), 0.5f * math::rand()));
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
    pList[0] = new sphere(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(new constant_texture(glm::vec3(0.4f, 0.2f, 0.1f))));
    pList[1] = new sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
    pList[2] = new sphere(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f));
    glm::vec3 center(0.0f, -4.0f, 0.0f);
    pList[3] = new moving_sphere(
        center,
        center + glm::vec3(0.0f, 0.5f * math::rand(), 0.0f),
        0.0f,
        0.75f,
        1.0f,
        new Lambertian(new constant_texture(glm::vec3(math::rand() * math::rand(), math::rand() * math::rand(), math::rand() * math::rand()))));

    auto pBlack = new constant_texture(glm::vec3(0.0f, 0.0f, 0.0f));
    auto pWhite = new constant_texture(glm::vec3(1.0f, 1.0f, 1.0f));
    auto pChecker = new checker_texture(pBlack, pWhite);
    pList[4] = new sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(pChecker));

    //return new bvh_node(pList, num_objects, 0.0f, 1.0f);
    return new object_list(pList, num_objects);
}

object* make_two_perlin_spheres()
{
    int num_objects = 2;
    auto pList = new object*[num_objects];
    auto pNoise = new noise_texture(true, 1.0f);
    pList[0] = new sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, new Lambertian(pNoise));
    pList[1] = new sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2, new Lambertian(pNoise));

    return new bvh_node(pList, num_objects, 0.0f, 1.0f);
}

object* make_two_spheres()
{
    int num_objects = 2;
    auto pList = new object*[num_objects];
    auto pBlue = new constant_texture(glm::vec3(0.0f, 0.0f, 0.6f));
    auto pGreen = new constant_texture(glm::vec3(0.0f, 0.6f, 0.0f));
    auto pChecker = new checker_texture(pBlue, pGreen);
    auto pRevChecker = new checker_texture(pGreen, pBlue);
    pList[0] = new sphere(glm::vec3(0.0f, -10.0f, 0.0f), 10, new Lambertian(pChecker));
    pList[1] = new sphere(glm::vec3(0.0f, 10.0f, 0.0f), 10, new Lambertian(pRevChecker));

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
    pList[0] = new sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2, new Lambertian(pImageTexture));
    auto pNoise = new noise_texture(true, 1.0f);
    pList[1] = new sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, new Lambertian(pNoise));
    return new bvh_node(pList, num_objects, 0.0f, 1.0f);
}

object* make_simple_light()
{
    int num_objects = 4;
    auto pList = new object*[num_objects];
    auto pPerlinTexture = new noise_texture(true, 4.0f);
    pList[0] = new sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(pPerlinTexture));
    pList[1] = new sphere(glm::vec3(0, 2, 0), 2, new Lambertian(pPerlinTexture));
    pList[2] = new sphere(glm::vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(glm::vec3(4, 4, 4))));
    pList[3] = new xy_rectangle(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(glm::vec3(4, 4, 4))));
    return new object_list(pList, num_objects);
}

object* make_cornell_box()
{
    int num_objects = 8;
    auto pList = new object*[num_objects];
    material* pRed = new Lambertian(new constant_texture(glm::vec3(0.65f, 0.05f, 0.05f)));
    material* pWhite = new Lambertian(new constant_texture(glm::vec3(0.73f, 0.73f, 0.73f)));
    material* pGreen = new Lambertian(new constant_texture(glm::vec3(0.12f, 0.45f, 0.15f)));
    material* pLight = new diffuse_light(new constant_texture(glm::vec3(15.0f, 15.0f, 15.0f)));
    
    pList[0] = new flip_normals(new yz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, pGreen));
    pList[1] = new yz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, pRed);
    pList[2] = new xz_rectangle(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, pLight);
    pList[3] = new flip_normals(new xz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, pWhite));
    pList[4] = new xz_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, pWhite);
    pList[5] = new flip_normals(new xy_rectangle(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, pWhite));
    pList[6] = new translate(new rotate(new box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 165.0f, 165.0f), pWhite), 0.0f, glm::radians(-18.0f), 0.0f), glm::vec3(130.0f, 0.0f, 65.0f));
    pList[7] = new translate(new rotate(new box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 330.0f, 165.0f), pWhite), 0.0f, glm::radians(15.0f), 0.0f), glm::vec3(265.0f, 0.0f, 295.0f));

    return new object_list(pList, num_objects);
}

object* make_cornell_smoke()
{
    int num_objects = 8;
    auto pList = new object*[num_objects];
    material *red = new Lambertian( new constant_texture(glm::vec3(0.65, 0.05, 0.05)) );
    material *white = new Lambertian( new constant_texture(glm::vec3(0.73, 0.73, 0.73)) );
    material *green = new Lambertian( new constant_texture(glm::vec3(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(glm::vec3(7, 7, 7)) );
    pList[0] = new flip_normals(new yz_rectangle(0, 555, 0, 555, 555, green));
    pList[1] = new yz_rectangle(0, 555, 0, 555, 0, red);
    pList[2] = new xz_rectangle(113, 443, 127, 432, 554, light);
    pList[3] = new flip_normals(new xz_rectangle(0, 555, 0, 555, 555, white));
    pList[4] = new xz_rectangle(0, 555, 0, 555, 0, white);
    pList[5] = new flip_normals(new xy_rectangle(0, 555, 0, 555, 555, white));
    object *b1 = new translate(new rotate(new box(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), 0.0f, glm::radians(-18.0f), 0.0f), glm::vec3(130,0,65));
    object *b2 = new translate(new rotate(new box(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white), 0.0f, glm::radians(15.0f), 0.0f), glm::vec3(265,0,295));
    pList[6] = new constant_medium(b1, 0.01, new constant_texture(glm::vec3(1.0, 1.0, 1.0)));
    pList[7] = new constant_medium(b2, 0.01, new constant_texture(glm::vec3(0.0, 0.0, 0.0)));
    
    return new object_list(pList, num_objects);
}

object* make_final() {
    int nb = 20;
    object **list = new object*[30];
    object **boxlist = new object*[10000];
    object **boxlist2 = new object*[10000];
    material *white = new Lambertian( new constant_texture(glm::vec3(0.73, 0.73, 0.73)) );
    material *ground = new Lambertian( new constant_texture(glm::vec3(0.48, 0.83, 0.53)) );
    int b = 0;
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < nb; j++) {
            float w = 100;
            float x0 = -1000 + i*w;
            float z0 = -1000 + j*w;
            float y0 = 0;
            float x1 = x0 + w;
            float y1 = 100*(math::rand()+0.01f);
            float z1 = z0 + w;
            boxlist[b++] = new box(glm::vec3(x0,y0,z0), glm::vec3(x1,y1,z1), ground);
        }
    }
    int l = 0;
    list[l++] = new bvh_node(boxlist, b, 0, 1);
    material *light = new diffuse_light( new constant_texture(glm::vec3(7, 7, 7)) );
    list[l++] = new xz_rectangle(123, 423, 147, 412, 554, light);
    glm::vec3 center(400, 400, 200);
    list[l++] = new moving_sphere(center, center+glm::vec3(30, 0, 0), 0, 1, 50, new Lambertian(new constant_texture(glm::vec3(0.7, 0.3, 0.1))));
    list[l++] = new sphere(glm::vec3(260, 150, 45), 50, new dielectric(1.5));
    list[l++] = new sphere(glm::vec3(0, 150, 145), 50, new metal(glm::vec3(0.8, 0.8, 0.9), 10.0));
    object *boundary = new sphere(glm::vec3(360, 150, 145), 70, new dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new constant_medium(boundary, 0.2, new constant_texture(glm::vec3(0.2, 0.4, 0.9)));
    boundary = new sphere(glm::vec3(0, 0, 0), 5000, new dielectric(1.5));
    list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(glm::vec3(1.0, 1.0, 1.0)));
    int nx, ny, nn;
    unsigned char* tex_data =
        stbi_load("E:/Projects/ray_tracing_in_a_weekend/physical_earth_satellite_image_mural_lg.jpg", &nx, &ny, &nn, 0);
    material *emat =  new Lambertian(new image_texture(tex_data, nx, ny));
    list[l++] = new sphere(glm::vec3(400,200, 400), 100, emat);
    texture *pertext = new noise_texture(true, 0.1f);
    list[l++] =  new sphere(glm::vec3(220,280, 300), 80, new Lambertian( pertext ));
    int ns = 1000;
    for (int j = 0; j < ns; j++) 
    {
        boxlist2[j] = new sphere(glm::vec3(165*math::rand(), 165*math::rand(), 165*math::rand()), 10, white);
    }
    list[l++] =   new translate(new rotate(new bvh_node(boxlist2,ns, 0.0, 1.0), 0.0f, 15.0f, 0.0f), glm::vec3(-100,270,395));
    
    return new object_list(list, l);
}

void create_image_synchronous(const int height, const int width, const int num_samples, const object* obj_list, camera& cam, std::vector<uint8_t>& image)
{
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            glm::vec3 color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < num_samples; s++)
            {
                float u = float(i + math::rand()) / float(width);
                float v = float(j + math::rand()) / float(height);
                ray r = cam.get_ray(u, v);

                color += linear_interp_color(r, obj_list, 0);
            }
            color /= num_samples;
            color = glm::vec3(std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]));
            auto ir = uint8_t(255.99f * color.r);
            auto ig = uint8_t(255.99f * color.g);
            auto ib = uint8_t(255.99f * color.b);
            image.push_back(ir);
            image.push_back(ig);
            image.push_back(ib);
        }
    }
}

void create_image_asynchronous(const int height, const int width, const int num_samples, const object* obj_list, camera& cam, std::vector<uint8_t>& image)
{
    const int max = width*height;
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> future_vector;
    auto pPixels = new glm::vec3[max];
    while (cores--)
    {
        future_vector.push_back(
            std::async([=, &pPixels, &count, &cam]()
            {
                while (true)
                {
                    std::size_t index = count++;
                    if (index >= max)
                        break;
                    std::size_t x = index % width;
                    std::size_t y = index / width;

                    glm::vec3 color(0.0f, 0.0f, 0.0f);
                    for (int s = 0; s < num_samples; s++)
                    {
                        float u = float(x + math::rand()) / float(width);
                        float v = float(y + math::rand()) / float(height);
                        ray r = cam.get_ray(u, v);

                        color += linear_interp_color(r, obj_list, 0);
                    }
                    color /= num_samples;
                    color = glm::vec3(std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]));

                    pPixels[index] = color;
                }
            }));
    }
    for(auto & future : future_vector)
    {
        future.get();
    }
    for(int i = max-1; i != 0; i--)
    {
        glm::vec3 color = pPixels[i];
        auto ir = uint8_t(255.99f * color.r);
        auto ig = uint8_t(255.99f * color.g);
        auto ib = uint8_t(255.99f * color.b);
        image.push_back(ir);
        image.push_back(ig);
        image.push_back(ib);
    }
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

    //object* obj_list = make_two_spheres();
    //object* obj_list = make_test_world();
    //object* obj_list = make_random_world();
    //object* obj_list = make_textured_sphere();
    //object* obj_list = make_simple_light();
    //object* obj_list = make_cornell_box();
    //object* obj_list = make_cornell_smoke();
    object* obj_list = make_final();

    //vec3 lookfrom(0.0f, 2.0f, 10.0f);
    //vec3 lookat(0.0f, 0.0f, 0.0f);
    glm::vec3 lookfrom(278.0f, 278.0f, -800.0f);
    glm::vec3 lookat(278.0f, 278.0f, 0.0f);
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    float fov = 40.0f;

    camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), fov, float(width) / float(height), aperture, dist_to_focus, 0.0f, 1.0f);

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    //create_image_synchronous(height, width, num_samples, obj_list, cam, image);
    create_image_asynchronous(height, width, num_samples, obj_list, cam, image);

    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << time_span.count() << " seconds.";

    stbi_write_png("test.png", width, height, comp, image.data(), stride);
}