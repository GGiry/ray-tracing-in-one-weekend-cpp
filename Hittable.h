#ifndef RAY_TRACING_IN_CPP_HITTABLE_H
#define RAY_TRACING_IN_CPP_HITTABLE_H

#include <utility>

#include "aabb.h"
#include "util.h"

class Material;

struct Hit_record {
    Point3 point;
    Vec3 normal;
    shared_ptr<Material> material_ptr;
    double t = 0.0;
    double u;
    double v;
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

class Translate : public Hittable {
public:
    Vec3 offset;
    shared_ptr<Hittable> object;

    Translate(shared_ptr<Hittable> _object, const Vec3 &vector) : offset(vector), object(std::move(_object)) {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override;
};

bool Translate::hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const {
    Ray moved_ray(ray.origin() - offset, ray.direction(), ray.time());
    if (!object->hit(moved_ray, t_min, t_max, rec)) { return false; }

    rec.point += offset;
    rec.set_face_normal(moved_ray, rec.normal);

    return true;
}

bool Translate::bounding_box(double time0, double time1, AABB &output_box) const {
    if (!object->bounding_box(time0, time1, output_box)) { return false; }

    output_box = AABB(output_box.min() + offset, output_box.max() + offset);

    return true;
}

#endif //RAY_TRACING_IN_CPP_HITTABLE_H
