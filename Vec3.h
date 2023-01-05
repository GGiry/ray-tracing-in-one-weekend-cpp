
#ifndef RAY_TRACING_IN_CPP_VEC3_H
#define RAY_TRACING_IN_CPP_VEC3_H

#include <cmath>
#include <iostream>
#include <array>

using std::sqrt;

class Vec3 {
public:
    std::array<double, 3> coordinates;

    Vec3() : coordinates{0, 0, 0} {}

    Vec3(double x, double y, double z) : coordinates{x, y, z} {}

    [[nodiscard]] double x() const { return coordinates[0]; }

    [[nodiscard]] double y() const { return coordinates[1]; }

    [[nodiscard]] double z() const { return coordinates[2]; }

    Vec3 operator-() const { return {-coordinates[0], -coordinates[1], -coordinates[2]}; }

    double operator[](int i) const { return coordinates[i]; }

    double &operator[](int i) { return coordinates[i]; }

    Vec3 &operator+=(const Vec3 &other) {
        coordinates[0] += other[0];
        coordinates[1] += other[1];
        coordinates[2] += other[2];

        return *this;
    }

    Vec3 &operator*=(const double t) {
        coordinates[0] *= t;
        coordinates[1] *= t;
        coordinates[2] *= t;

        return *this;
    }

    Vec3 &operator/=(const double t) {
        return *this *= 1 / t;
    }

    [[nodiscard]] double length() const {
        return sqrt(length_squared());
    }

    [[nodiscard]] double length_squared() const {
        return coordinates[0] * coordinates[0] + coordinates[1] * coordinates[1] + coordinates[2] * coordinates[2];
    }
};

// Type aliases for Vec3
using Point3 = Vec3;
using Color = Vec3;


#endif //RAY_TRACING_IN_CPP_VEC3_H
