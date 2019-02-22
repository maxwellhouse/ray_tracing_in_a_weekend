#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb/stb_image_write.h"

#include "object_list.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#include <iostream>

vec3 linear_interp_color(const ray& r, const object* obj, const int depth)
{
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    hit_record rec;
    if(obj->hit(r, 0.001, std::numeric_limits<float>::max(), rec))
    {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.pMaterial->scatter(r, rec, attenuation, scattered))
        { 
            color = attenuation * linear_interp_color(scattered, obj, depth + 1);
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

object* make_random_world()
{
    int i = 0;
    int num_objects = 11;
    object** pList = new object*[482];
    pList[i++] = new sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5f, 0.5f, 0.5f)));

    for (int a = -num_objects; a < num_objects; a++)
    {
        for (int b = -num_objects; b < num_objects; b++)
        {
            float choose_material = math::distribution(math::random_number);
            vec3 center(a + 0.9f*math::distribution(math::random_number), 0.2f, b + 0.9f*math::distribution(math::random_number));
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) 
            {
                if (choose_material < 0.8f) 
                {  // diffuse
                    if (math::random_number() % 2)
                    {
                        pList[i++] = new sphere(center,
                                        0.2f,
                                        new Lambertian(vec3(math::distribution(math::random_number)*math::distribution(math::random_number), math::distribution(math::random_number)*math::distribution(math::random_number), math::distribution(math::random_number)*math::distribution(math::random_number))));
                    }
                    else
                    {
                        pList[i++] = new moving_sphere(center,
                            center + vec3(0.0f, 0.5f * math::distribution(math::random_number), 0.0f),
                            0.0f,
                            1.0f,
                            0.2f,
                            new Lambertian(vec3(math::distribution(math::random_number)*math::distribution(math::random_number), math::distribution(math::random_number)*math::distribution(math::random_number), math::distribution(math::random_number)*math::distribution(math::random_number))));
                    }
                }
                else if (choose_material < 0.95f) 
                { // metal
                    pList[i++] = new sphere(center, 0.2f,
                        new metal(vec3(0.5f*(1 + math::distribution(math::random_number)), 0.5f*(1 + math::distribution(math::random_number)), 0.5f*(1 + math::distribution(math::random_number))), 0.5f*math::distribution(math::random_number)));
                }
                else 
                {  // glass
                    pList[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
                }
            }
        }
    }
    return new object_list(pList, i);
}

object* make_test_world()
{
    int num_objects = 4;
    object** pList = new object*[num_objects];
    pList[0] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(vec3(0.4f, 0.2f, 0.1f)));
    pList[1] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
    pList[2] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));
    vec3 center(0.0f, -4.0f, 0.0f);
    pList[3] = new moving_sphere(center,
        center + vec3(0.0f, 0.5f*math::distribution(math::random_number), 0.0f),
        0.0f,
        0.75f,
        1.0f,
        new Lambertian(vec3(math::distribution(math::random_number)*math::distribution(math::random_number), math::distribution(math::random_number)*math::distribution(math::random_number), math::distribution(math::random_number)*math::distribution(math::random_number))));

    return new object_list(pList, num_objects);
}

int main()
{
    const int width = 800;
    const int height = 600;
    const int ns = 10;
    int comp = 3;
    std::vector<uint8_t> image;
    const int bitsPerPixel = comp * (8* sizeof(uint8_t));
    int stride = width * bitsPerPixel;  // bits per row
    stride += 31;            // round up to next 32-bit boundary
    stride /= 32;            // DWORDs per row
    stride *= 4;             // bytes per row

    //object* obj_list = make_test_world();
    object* obj_list = make_random_world();

    vec3 lookfrom(13.0f, 2.0f, 30.0f);
    vec3 lookat(0.0f, 0.0f, 0.0f);
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    float fov = 20.0f;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), fov, float(width) / float(height), aperture, dist_to_focus, 0.0f, 1.0f);

    for(int j = height-1; j >= 0; j--)
    {
        std::cout << j << "\n";
        for(int i = 0; i < width; i++)
        {
            vec3 color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + math::distribution(math::random_number)) / float(width);
                float v = float(j + math::distribution(math::random_number)) / float(height);
                ray r = cam.get_ray(u, v);

                color += linear_interp_color(r, obj_list, 0);
            }
            color /= float(ns);
            color = vec3(sqrtf(color[0]), sqrtf(color[1]), sqrtf(color[2]));
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
