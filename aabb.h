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
            auto t0 = fmin((minimum[a] - ray.origin()[a]) / ray.direction()[a],
                           (maximum[a] - ray.origin()[a]) / ray.direction()[a]);
            auto t1 = fmax((minimum[a] - ray.origin()[a]) / ray.direction()[a],
                           (maximum[a] - ray.origin()[a]) / ray.direction()[a]);

            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);

            if (t_max <= t_min) {
                return false;
            }
        }

        return true;
    }

};

#endif //RAY_TRACING_IN_CPP_AABB_H
