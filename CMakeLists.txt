cmake_minimum_required(VERSION 3.13)
project(raytracer VERSION 1.0.0 LANGUAGES CXX)

set(SFML_DIR "./external/sfml/build")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

find_package(glm 0.9.9 COMPONENTS glm REQUIRED)

add_executable( raytracer
        src/main.cpp
        src/camera.h

        src/materials/dielectric.h
        src/materials/diffuse_light.h
        src/materials/lambertian.h
        src/materials/material.h
        src/materials/metal.h
        src/materials/isotropic.h

        src/math/math.h
        src/math/ray.h
        src/math/perlin_noise.h

        src/objects/flip_normals.h
        src/objects/rectangle.h
        src/objects/aabb.h
        src/objects/bvh_node.h
        src/objects/moving_sphere.h
        src/objects/object.h
        src/objects/object_list.h
        src/objects/sphere.h
        src/objects/box.h
        src/objects/translate.h
        src/objects/rotate.h

        src/textures/image_texture.h
        src/textures/noise_texture.h
        src/textures/checker_texture.h
        src/textures/constant_texture.h
        src/textures/texture.h
        )

target_include_directories(raytracer
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
        )

target_compile_features(raytracer PRIVATE cxx_std_11)
target_compile_options(raytracer PRIVATE -W4)

if(LINUX)
        target_compile_options(raytracer PRIVATE -Werror)
        target_link_libraries(raytracer m dl sfml-graphics sfml-window sfml-system glm)
endif (LINUX)
if(MSVC)
        target_compile_options(raytracer PRIVATE -WX)
        target_link_libraries(raytracer sfml-graphics sfml-window sfml-system glm)
endif (MSVC)

add_custom_command(TARGET raytracer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/external/sfml/build/lib/$<CONFIGURATION>/sfml-system-d-2.dll"
        "${PROJECT_SOURCE_DIR}/external/sfml/build/lib/$<CONFIGURATION>/sfml-graphics-d-2.dll"
        "${PROJECT_SOURCE_DIR}/external/sfml/build/lib/$<CONFIGURATION>/sfml-window-d-2.dll"
        $<TARGET_FILE_DIR:raytracer>)
