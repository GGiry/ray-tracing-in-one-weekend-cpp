#ifndef RAY_TRACING_IN_CPP_AABB_H
#define RAY_TRACING_IN_CPP_AABB_H

#include "util.h"

class Axis_Aligned_Bounding_Box {
public:
    Point3 minimum;
    Point3 maximum;

    Axis_Aligned_Bounding_Box() = default;

    Axis_Aligned_Bounding_Box(const Point3 &point_a, const Point3 &point_b) : minimum(point_a), maximum(point_b) {};

    [[nodiscard]] Point3 min() const { return minimum; }

    [[nodiscard]] Point3 max() const { return maximum; }

    [[nodiscard]] bool hit(const Ray &ray, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto inverse_direction = 1.0 / ray.direction()[a];

            auto t0 = (min()[a] - ray.origin()[a]) * inverse_direction;
            auto t1 = (max()[a] - ray.origin()[a]) * inverse_direction;

            if (inverse_direction < 0.0) { std::swap(t0, t1); }

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;

            if (t_max <= t_min) {
                return false;
            }
        }

        return true;
    }

};

using AABB = Axis_Aligned_Bounding_Box;

inline AABB surrounding_box(const AABB &box0, const AABB &box1) {
    Point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    Point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return {small, big};
}

#endif //RAY_TRACING_IN_CPP_AABB_H
