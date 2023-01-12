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

#endif //RAY_TRACING_IN_CPP_AA_RECTANGLE_H
