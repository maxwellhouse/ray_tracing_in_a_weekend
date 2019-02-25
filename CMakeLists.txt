cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project("Ray tracing in a weekend")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    source_list
    main.cpp
    camera.h
    
    materials/dielectric.h
    materials/lambertian.h
    materials/material.h
    materials/metal.h
    
    math/math.h
    math/ray.h
    math/vec3.h
    
    objects/aabb.h
    objects/bvh_node.h
    objects/moving_sphere.h
    objects/object.h
    objects/object_list.h
    objects/sphere.h
    
    textures/checker_texture.h
    textures/constant_texture.h
    textures/texture.h
)

add_executable(raytracer ${source_list})

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
    
target_include_directories(raytracer
    PRIVATE
        ../materials
        ../math
        ../objects
        ../textures
)

if(LINUX)
    target_link_libraries(raytracer m dl)
endif (LINUX)
if(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")
    target_link_libraries(raytracer)
endif (MSVC)