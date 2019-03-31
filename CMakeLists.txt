cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project("raytracer")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable( raytracer
    main.cpp
    camera.h
    
    materials/dielectric.h
	materials/diffuse_light.h
    materials/lambertian.h
    materials/material.h
    materials/metal.h
    
    math/math.h
    math/ray.h
    math/vec3.h
    math/perlin_noise.h
    
    objects/flip_normals.h
	objects/rectangle.h
    objects/aabb.h
    objects/bvh_node.h
    objects/moving_sphere.h
    objects/object.h
    objects/object_list.h
    objects/sphere.h
    
	textures/image_texture.h
    textures/noise_texture.h
    textures/checker_texture.h
    textures/constant_texture.h
    textures/texture.h
)

target_include_directories(raytracer
    PRIVATE
        ./thirdparty/
)

if(LINUX)
    target_link_libraries(raytracer m dl)
endif (LINUX)
if(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")
    target_link_libraries(raytracer)
endif (MSVC)