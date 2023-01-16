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

class Rotate_y : public Hittable {
public:
    shared_ptr<Hittable> object;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    AABB bbox;

    Rotate_y(const shared_ptr<Hittable> &_object, double angle);

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = bbox;
        return hasbox;
    }
};

Rotate_y::Rotate_y(const shared_ptr<Hittable> &_object, double angle) : object(_object) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = _object->bounding_box(0, 1, bbox);

    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                auto new_x = cos_theta * x + sin_theta * z;
                auto new_z = -sin_theta * x + cos_theta * z;

                Vec3 tester(new_x, y, new_z);

                for (int c = 0; c < 3; ++c) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }
    bbox = AABB(min, max);
}

bool Rotate_y::hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const {
    auto origin = ray.origin();
    auto direction = ray.direction();

    origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
    origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];

    direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
    direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];

    Ray rotated_ray(origin, direction, ray.time());

    if (!object->hit(rotated_ray, t_min, t_max, rec)) { return false; }

    auto point = rec.point;
    auto normal = rec.normal;

    point[0] = cos_theta * rec.point[0] + sin_theta * rec.point[2];
    point[2] = -sin_theta * rec.point[0] + cos_theta * rec.point[2];

    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.point = point;
    rec.set_face_normal(rotated_ray, normal);

    return true;
}

#endif //RAY_TRACING_IN_CPP_HITTABLE_H
