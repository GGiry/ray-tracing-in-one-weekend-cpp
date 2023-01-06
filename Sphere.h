#ifndef RAY_TRACING_IN_CPP_SPHERE_H
#define RAY_TRACING_IN_CPP_SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
public:
    Point3 _center;
    double _radius = 0.0;

    Sphere() = default;

    Sphere(Point3 center, double radius) : _center(center), _radius(radius) {};

    bool hit(const Ray &r, double t_min, double t_max, Hit_record &record) const override;
};

bool Sphere::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    Vec3 origin_center = ray.origin() - _center;

    auto a = ray.direction().length_squared();
    auto half_b = dot(origin_center, ray.direction());
    auto c = origin_center.length_squared() - _radius * _radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) { return false; }
    auto sqrt_discriminant = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrt_discriminant) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }

    record.t = root;
    record.point = ray.at(root);
    record.normal = (record.point - _center) / _radius;

    return true;
}


#endif //RAY_TRACING_IN_CPP_SPHERE_H
