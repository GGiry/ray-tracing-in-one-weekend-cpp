#ifndef RAY_TRACING_IN_CPP_AA_RECTANGLE_H
#define RAY_TRACING_IN_CPP_AA_RECTANGLE_H

#include "util.h"

#include "Hittable.h"

class xy_rectangle : public Hittable {
public:
    shared_ptr<Material> material;
    double x0 = 0.0;
    double x1 = 0.0;
    double y0 = 0.0;
    double y1 = 0.0;
    double k = 0.0;

    xy_rectangle() = default;

    xy_rectangle(double _x0, double _x1, double _y0, double _y1, double _k, const shared_ptr<Material> &mat)
            : material(mat), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount.
        output_box = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
        return true;
    }
};

class xz_rectangle : public Hittable {
public:
    shared_ptr<Material> material;
    double x0 = 0.0;
    double x1 = 0.0;
    double z0 = 0.0;
    double z1 = 0.0;
    double k = 0.0;

    xz_rectangle() = default;

    xz_rectangle(double _x0, double _x1, double _z0, double _z1, double _k, const shared_ptr<Material> &mat)
            : material(mat), x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount.
        output_box = AABB(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
        return true;
    }
};

class yz_rectangle : public Hittable {
public:
    shared_ptr<Material> material;
    double y0 = 0.0;
    double y1 = 0.0;
    double z0 = 0.0;
    double z1 = 0.0;
    double k = 0.0;

    yz_rectangle() = default;

    yz_rectangle(double _y0, double _y1, double _z0, double _z1, double _k, const shared_ptr<Material> &mat)
            : material(mat), y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount.
        output_box = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
        return true;
    }
};

bool xy_rectangle::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    auto t = (k - ray.origin().z()) / ray.direction().z();
    if (t < t_min || t > t_max) { return false; }

    auto x = ray.origin().x() + t * ray.direction().x();
    auto y = ray.origin().y() + t * ray.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1) { return false; }

    record.u = (x - x0) / (x1 - x0);
    record.v = (y - y0) / (y1 - y0);
    record.t = t;

    auto outward_normal = Vec3(0, 0, 1);
    record.set_face_normal(ray, outward_normal);
    record.material_ptr = material;
    record.point = ray.at(t);

    return true;
}

bool xz_rectangle::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    auto t = (k - ray.origin().y()) / ray.direction().y();
    if (t < t_min || t > t_max) { return false; }

    auto x = ray.origin().x() + t * ray.direction().x();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1) { return false; }

    record.u = (x - x0) / (x1 - x0);
    record.v = (z - z0) / (z1 - z0);
    record.t = t;

    auto outward_normal = Vec3(0, 1, 0);
    record.set_face_normal(ray, outward_normal);
    record.material_ptr = material;
    record.point = ray.at(t);

    return true;
}

bool yz_rectangle::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    auto t = (k - ray.origin().x()) / ray.direction().x();
    if (t < t_min || t > t_max) { return false; }

    auto y = ray.origin().y() + t * ray.direction().y();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1) { return false; }

    record.u = (y - y0) / (y1 - y0);
    record.v = (z - z0) / (z1 - z0);
    record.t = t;

    auto outward_normal = Vec3(1, 0, 0);
    record.set_face_normal(ray, outward_normal);
    record.material_ptr = material;
    record.point = ray.at(t);

    return true;
}

#endif //RAY_TRACING_IN_CPP_AA_RECTANGLE_H
