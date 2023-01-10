#ifndef RAY_TRACING_IN_CPP_SPHERE_H
#define RAY_TRACING_IN_CPP_SPHERE_H

#include <utility>

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
public:
    Point3 _center;
    double _radius = 0.0;
    std::shared_ptr<Material> _material_ptr;

    Sphere() = default;

    Sphere(Point3 center, double radius, shared_ptr<Material> material) : _center(center), _radius(radius),
                                                                          _material_ptr(std::move(material)) {};

    bool hit(const Ray &r, double t_min, double t_max, Hit_record &record) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override;

private:
    static void get_sphere_uv(const Point3 &point, double &u, double &v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-point.y());
        auto phi = atan2(-point.z(), point.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
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
    Vec3 outward_normal = (record.point - _center) / _radius;
    record.set_face_normal(ray, outward_normal);
    get_sphere_uv(outward_normal, record.u, record.v);
    record.material_ptr = _material_ptr;

    return true;
}

bool Sphere::bounding_box(double time0, double time1, AABB &output_box) const {
    output_box = AABB(_center - Vec3(_radius, _radius, _radius),
                      _center + Vec3(_radius, _radius, _radius));
    return true;
}

#endif //RAY_TRACING_IN_CPP_SPHERE_H
