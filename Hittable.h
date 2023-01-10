#ifndef RAY_TRACING_IN_CPP_HITTABLE_H
#define RAY_TRACING_IN_CPP_HITTABLE_H

#include "aabb.h"
#include "util.h"

class Material;

struct Hit_record {
    Point3 point;
    Vec3 normal;
    shared_ptr<Material> material_ptr;
    double t = 0.0;
    bool front_face = false;

    inline void set_face_normal(const Ray &ray, const Vec3 &outward_normal) {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const = 0;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const = 0;
};


#endif //RAY_TRACING_IN_CPP_HITTABLE_H
