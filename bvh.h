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

bool BVH_node::hit(const Ray &ray, double t_min, double t_max, Hit_record &record) const {
    if (!box.hit(ray, t_min, t_max)) { return false; }

    bool hit_left = left->hit(ray, t_min, t_max, record);
    bool hit_right = right->hit(ray, t_min, hit_left ? record.t : t_max, record);

    return hit_left || hit_right;
}

BVH_node::Bounding_Volume_Hierarchy_node(const vector<shared_ptr<Hittable>> &src_objects,
                                         size_t start, size_t end, double time0, double time1) {
    auto objects = src_objects; // modifiable array of the source

    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare
                                  : (axis == 1) ? box_y_compare
                                                : box_z_compare;


    if (size_t object_span = end - start; object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        auto mid = start + object_span / 2;
        left = std::make_shared<BVH_node>(objects, start, mid, time0, time1);
        right = std::make_shared<BVH_node>(objects, mid, end, time0, time1);
    }

    AABB box_left;
    AABB box_right;

    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    box = surrounding_box(box_left, box_right);
}


#endif //RAY_TRACING_IN_CPP_BVH_H
