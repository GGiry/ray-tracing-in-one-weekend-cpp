#ifndef RAY_TRACING_IN_CPP_BOX_H
#define RAY_TRACING_IN_CPP_BOX_H

#include "util.h"

#include "aa_rectangle.h"
#include "Hittable_list.h"

class Box : public Hittable {
public:
    Point3 box_min;
    Point3 box_max;
    Hittable_list sides;

    Box() = default;

    Box(const Point3 &p0, const Point3 &p1, const std::shared_ptr<Material> &material);

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = AABB(box_min, box_max);
        return true;
    }
};

Box::Box(const Point3 &p0, const Point3 &p1, const std::shared_ptr<Material> &material) : box_min(p0), box_max(p1) {
    sides.add(std::make_shared<xy_rectangle>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
    sides.add(std::make_shared<xy_rectangle>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material));

    sides.add(std::make_shared<xz_rectangle>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
    sides.add(std::make_shared<xz_rectangle>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material));

    sides.add(std::make_shared<yz_rectangle>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
    sides.add(std::make_shared<yz_rectangle>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material));
}

bool Box::hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const {
    return sides.hit(ray, t_min, t_max, rec);
}

#endif //RAY_TRACING_IN_CPP_BOX_H
