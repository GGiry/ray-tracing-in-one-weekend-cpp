#ifndef RAY_TRACING_IN_CPP_HITTABLE_H
#define RAY_TRACING_IN_CPP_HITTABLE_H

#include "Ray.h"

struct Hit_record {
    Point3 point;
    Vec3 normal;
    double t{};
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const = 0;
};


#endif //RAY_TRACING_IN_CPP_HITTABLE_H
