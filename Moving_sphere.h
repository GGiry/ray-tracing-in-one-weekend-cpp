#ifndef RAY_TRACING_IN_CPP_MOVING_SPHERE_H
#define RAY_TRACING_IN_CPP_MOVING_SPHERE_H

#include <utility>

#include "util.h"
#include "Hittable.h"

class Moving_sphere : public Hittable {
public:
    Point3 center0;
    Point3 center1;
    double time0 = 0.0;
    double time1 = 0.0;
    double radius = 0.0;
    shared_ptr<Material> material_ptr;

    Moving_sphere() = default;

    Moving_sphere(Point3 center_begin, Point3 center_end, double time_begin, double time_end, double r,
                  shared_ptr<Material> material) :
            center0(center_begin), center1(center_end), time0(time_begin), time1(time_end), radius(r),
            material_ptr(std::move(material)) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const override;

    bool bounding_box(double time_0, double time_1, AABB &output_box) const override;

    [[nodiscard]] Point3 center(double time) const;
};

Point3 Moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool Moving_sphere::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    Vec3 origin_center = ray.origin() - center(ray.time());
    auto a = ray.direction().length_squared();
    auto half_b = dot(origin_center, ray.direction());
    auto c = origin_center.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) { return false; }
    auto sqrt_discriminant = sqrt(discriminant);

    // Find the nearest root that lis in the acceptable range.
    auto root = (-half_b - sqrt_discriminant) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }

    record.t = root;
    record.point = ray.at(root);
    auto outward_normal = (record.point - center(ray.time())) / radius;
    record.set_face_normal(ray, outward_normal);
    record.material_ptr = material_ptr;

    return true;
}

bool Moving_sphere::bounding_box(double time_0, double time_1, AABB &output_box) const {
    AABB box0(center(time_0) - Vec3(radius, radius, radius),
              center(time_0) + Vec3(radius, radius, radius));

    AABB box1(center(time_1) - Vec3(radius, radius, radius),
              center(time_1) + Vec3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);
    return true;
}

#endif //RAY_TRACING_IN_CPP_MOVING_SPHERE_H
