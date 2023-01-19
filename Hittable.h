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

class Rotate : public Hittable {
public:
    shared_ptr<Hittable> object;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    AABB bbox;

    Rotate(const shared_ptr<Hittable> &_object, double angle);

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = bbox;
        return hasbox;
    }

    void compute_AABB();

private:
    [[nodiscard]] virtual inline Vec3 rotate(const Vec3 &vec) const = 0;

    [[nodiscard]] virtual inline Vec3 inverse_rotate(const Vec3 &vec) const = 0;
};

Rotate::Rotate(const shared_ptr<Hittable> &_object, double angle) : object(_object) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = _object->bounding_box(0, 1, bbox);
}

bool Rotate::hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const {
    auto origin = inverse_rotate(ray.origin());
    auto direction = inverse_rotate(ray.direction());

    Ray rotated_ray(origin, direction, ray.time());

    if (!object->hit(rotated_ray, t_min, t_max, rec)) { return false; }

    auto point = rotate(rec.point);
    auto normal = rotate(rec.normal);

    rec.point = point;
    rec.set_face_normal(rotated_ray, normal);

    return true;
}

void Rotate::compute_AABB() {
    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                Vec3 tester = rotate(Vec3(x, y, z));
                for (int c = 0; c < 3; ++c) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = AABB(min, max);
}


class Rotate_y : public Rotate {
public:
    Rotate_y(const shared_ptr<Hittable> &_object, double angle);

private:
    [[nodiscard]] inline Vec3 rotate(const Vec3 &vec) const override {
        double new_x = cos_theta * vec.x() + sin_theta * vec.z();
        double new_z = -sin_theta * vec.x() + cos_theta * vec.z();

        return {new_x, vec.y(), new_z};
    }

    [[nodiscard]] inline Vec3 inverse_rotate(const Vec3 &vec) const override {
        double new_x = cos_theta * vec.x() - sin_theta * vec.z();
        double new_z = sin_theta * vec.x() + cos_theta * vec.z();

        return {new_x, vec.y(), new_z};
    }
};

Rotate_y::Rotate_y(const shared_ptr<Hittable> &_object, double angle) : Rotate(_object, angle) {
    compute_AABB();
}


class Rotate_x : public Rotate {
public:

    Rotate_x(const shared_ptr<Hittable> &_object, double angle);

private:

    [[nodiscard]] inline Vec3 rotate(const Vec3 &vec) const override {
        double new_y = cos_theta * vec.y() - sin_theta * vec.z();
        double new_z = sin_theta * vec.y() + cos_theta * vec.z();

        return {vec.x(), new_y, new_z};
    }

    [[nodiscard]] inline Vec3 inverse_rotate(const Vec3 &vec) const override {
        double new_y = cos_theta * vec.y() + sin_theta * vec.z();
        double new_z = -sin_theta * vec.y() + cos_theta * vec.z();

        return {vec.x(), new_y, new_z};
    }
};

Rotate_x::Rotate_x(const shared_ptr<Hittable> &_object, double angle) : Rotate(_object, angle) {
    compute_AABB();
}

class Rotate_z : public Rotate {
public:
    Rotate_z(const shared_ptr<Hittable> &_object, double angle);

private:

    [[nodiscard]] inline Vec3 rotate(const Vec3 &vec) const override {
        double new_x = cos_theta * vec.x() - sin_theta * vec.y();
        double new_y = sin_theta * vec.x() + cos_theta * vec.y();

        return {new_x, new_y, vec.z()};
    }

    [[nodiscard]] inline Vec3 inverse_rotate(const Vec3 &vec) const override {
        double new_x = cos_theta * vec.x() + sin_theta * vec.y();
        double new_y = -sin_theta * vec.x() + cos_theta * vec.y();

        return {new_x, new_y, vec.z()};
    }
};

Rotate_z::Rotate_z(const shared_ptr<Hittable> &_object, double angle) : Rotate(_object, angle) {
    compute_AABB();
}

#endif //RAY_TRACING_IN_CPP_HITTABLE_H
