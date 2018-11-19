#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#include <vector>
#include <random>
#include <iostream>

bool hit_any_item_in_list(const std::vector<object*>& obj_list, const ray& r, hit_record& rec)
{
    hit_record temp_rec;
    bool hit_anything = false;
    float t_min = 0.001f;
    float closest_so_far = std::numeric_limits<float>::max();

    for (const auto& object : obj_list)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

vec3 linear_interp_color(const ray& r, const std::vector<object*>& obj_list, const int depth)
{
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    hit_record rec;
    if (hit_any_item_in_list(obj_list, r, rec))
    {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.pMaterial->scatter(r, rec, attenuation, scattered))
        { 
            color = attenuation * linear_interp_color(scattered, obj_list, depth + 1);
        }
    }
    else
    {
        vec3 unit_direction = make_unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        color = (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }

    return color;
}

void make_world(std::vector<object*>& obj_list)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> unif(0, 1);

    obj_list.push_back(new sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5f, 0.5f, 0.5f))));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_material = unif(gen);
            vec3 center(a + 0.9f*unif(gen), 0.2f, b + 0.9f*unif(gen));
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) 
            {
                if (choose_material < 0.8f) 
                {  // diffuse
                    //obj_list.push_back(new sphere(center, 0.2f, new Lambertian(vec3(unif(gen)*unif(gen), unif(gen)*unif(gen), unif(gen)*unif(gen)))));
                    obj_list.push_back(new moving_sphere(center, center + vec3(0.0f, 0.5f * unif(gen), 0.0f), 0.0f, 1.0f, 0.2f, new Lambertian(vec3(unif(gen)*unif(gen), unif(gen)*unif(gen), unif(gen)*unif(gen)))));
                }
                else if (choose_material < 0.95f) 
                { // metal
                    obj_list.push_back(new sphere(center, 0.2f,
                        new metal(vec3(0.5f*(1 + unif(gen)), 0.5f*(1 + unif(gen)), 0.5f*(1 + unif(gen))), 0.5f*unif(gen))));
                }
                else {  // glass
                    obj_list.push_back(new sphere(center, 0.2f, new dielectric(1.5f)));
                }
            }
        }
    }
    obj_list.push_back(new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f)));
    obj_list.push_back(new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(vec3(0.4f, 0.2f, 0.1f))));
    obj_list.push_back(new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f)));
}

int main()
{
    const int width = 1200;
    const int height = 800;
    const int ns = 10;
    int comp = 3;
    std::vector<uint8_t> image;
    const int bitsPerPixel = comp * (8* sizeof(uint8_t));
    int stride = width * bitsPerPixel;  // bits per row
    stride += 31;            // round up to next 32-bit boundary
    stride /= 32;            // DWORDs per row
    stride *= 4;             // bytes per row

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    std::vector<object*> obj_list;
    make_world(obj_list);

    vec3 lookfrom(13.0f, 2.0f, 3.0f);
    vec3 lookat(0.0f, 0.0f, 0.0f);
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    float fov = 20.0f;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), fov, float(width) / float(height), aperture, dist_to_focus, 0.0f, 1.0f);

    for(int j = height-1; j >= 0; j--)
    {
        for(int i = 0; i < width; i++)
        {
            vec3 color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + distribution(generator)) / float(width);
                float v = float(j + distribution(generator)) / float(height);
                ray r = cam.get_ray(u, v);

                color += linear_interp_color(r, obj_list, 0);
            }
            //std::cout << i << ":" << j << "\n";
            color /= float(ns);
            color = vec3(std::sqrtf(color[0]), std::sqrtf(color[1]), std::sqrtf(color[2]));
            uint8_t ir = uint8_t(255.99*color.r());
            uint8_t ig = uint8_t(255.99*color.g());
            uint8_t ib = uint8_t(255.99*color.b());
            image.push_back(ir);
            image.push_back(ig);
            image.push_back(ib);
        }
    }
    
    stbi_write_png("test.png", width, height, comp, image.data(), stride);
}
