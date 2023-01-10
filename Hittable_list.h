#ifndef RAY_TRACING_IN_CPP_HITTABLE_LIST_H
#define RAY_TRACING_IN_CPP_HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <utility>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::vector;

class Hittable_list : public Hittable {
public:
    vector<shared_ptr<Hittable>> objects;

    Hittable_list() = default;

    explicit Hittable_list(const shared_ptr<Hittable> &object) { add(object); }

    void clear() { objects.clear(); }

    void add(const shared_ptr<Hittable> &object) { objects.push_back(object); }

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override;
};

bool Hittable_list::hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const {
    Hit_record temp_record;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object: objects) {
        if (object->hit(ray, t_min, closest_so_far, temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            rec = temp_record;
        }
    }

    return hit_anything;
}

bool Hittable_list::bounding_box(double time0, double time1, AABB &output_box) const {
    if (objects.empty()) { return false; }

    AABB temporary_box;
    bool first_box = true;

    for (const auto &object: objects) {
        if (!object->bounding_box(time0, time1, temporary_box)) { return false; }
        output_box = first_box ? temporary_box : surrounding_box(output_box, temporary_box);
        first_box = false;
    }

    return true;
}


#endif //RAY_TRACING_IN_CPP_HITTABLE_LIST_H
