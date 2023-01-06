#ifndef RAY_TRACING_IN_CPP_RAY_H
#define RAY_TRACING_IN_CPP_RAY_H

#include "Vec3.h"

class Ray {
public:
    Point3 _origin;
    Vec3 _direction;

    Ray() = default;

    Ray(const Point3 &origin, const Vec3 &direction) : _origin(origin), _direction(direction) {}

    [[nodiscard]] Point3 origin() const { return _origin; }

    [[nodiscard]] Vec3 direction() const { return _direction; }

    [[nodiscard]] Point3 at(double t) const {
        return _origin + t * _direction;
    }
};


#endif //RAY_TRACING_IN_CPP_RAY_H
