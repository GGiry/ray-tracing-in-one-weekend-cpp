#ifndef RAY_TRACING_IN_CPP_BVH_H
#define RAY_TRACING_IN_CPP_BVH_H

#include "util.h"

#include "Hittable_list.h"

class Bounding_Volume_Hierarchy_node : public Hittable {
public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;

    Bounding_Volume_Hierarchy_node() = default;

    Bounding_Volume_Hierarchy_node(const Hittable_list &list, double time0, double time1) :
            Bounding_Volume_Hierarchy_node(list.objects, 0, list.objects.size(), time0, time1) {};

    Bounding_Volume_Hierarchy_node(const std::vector<shared_ptr<Hittable>> &src_objects, size_t start, size_t end,
                                   double time0, double time1);

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override;
};

using BVH_node = Bounding_Volume_Hierarchy_node;

bool BVH_node::bounding_box(double time0, double time1, AABB &output_box) const {
    output_box = box;
    return true;
}

bool Bounding_Volume_Hierarchy_node::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    if (!box.hit(ray, t_min, t_max)) { return false; }

    bool hit_left = left->hit(ray, t_min, t_max, record);
    bool hit_right = right->hit(ray, t_min, hit_left ? record.t : t_max, record);

    return hit_left || hit_right;
}

#endif //RAY_TRACING_IN_CPP_BVH_H
