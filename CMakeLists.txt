cmake_minimum_required(VERSION 3.24)
project(ray_tracing_in_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(ray_tracing_in_cpp main.cpp Vec3.h Color.h Ray.h Hittable.h Sphere.h Hittable_list.h util.h Camera.h Material.h Moving_sphere.h aabb.h bvh.h)
