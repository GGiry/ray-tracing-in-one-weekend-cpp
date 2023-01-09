#ifndef RAY_TRACING_IN_CPP_VEC3_H
#define RAY_TRACING_IN_CPP_VEC3_H

#include <cmath>
#include <iostream>
#include <array>

#include "util.h"

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

    inline static Vec3 random() {
        return {random_double(), random_double(), random_double()};
    }

    inline static Vec3 random(double min, double max) {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }

    [[nodiscard]] bool near_zero() const {
        const auto delta = 1e-8;
        return fabs(coordinates[0]) < delta && fabs(coordinates[1]) < delta && fabs(coordinates[2]) < delta;
    }
};

// Type aliases for Vec3
using Point3 = Vec3;
using Color = Vec3;


// Vec3 Utility functions

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.coordinates[0] << ' ' << v.coordinates[1] << ' ' << v.coordinates[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return {u.coordinates[0] + v.coordinates[0],
            u.coordinates[1] + v.coordinates[1],
            u.coordinates[2] + v.coordinates[2]};
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return {u.coordinates[0] - v.coordinates[0],
            u.coordinates[1] - v.coordinates[1],
            u.coordinates[2] - v.coordinates[2]};
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return {u.coordinates[0] * v.coordinates[0],
            u.coordinates[1] * v.coordinates[1],
            u.coordinates[2] * v.coordinates[2]};
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return {t * v.coordinates[0], t * v.coordinates[1], t * v.coordinates[2]};
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.coordinates[0] * v.coordinates[0]
           + u.coordinates[1] * v.coordinates[1]
           + u.coordinates[2] * v.coordinates[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return {u.coordinates[1] * v.coordinates[2] - u.coordinates[2] * v.coordinates[1],
            u.coordinates[2] * v.coordinates[0] - u.coordinates[0] * v.coordinates[2],
            u.coordinates[0] * v.coordinates[1] - u.coordinates[1] * v.coordinates[0]};
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

inline Vec3 random_in_unit_sphere() {
    while (true) {
        auto candidate = Vec3::random(-1, 1);
        if (candidate.length_squared() > 1) { continue; }
        return candidate;
    }
}

inline Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline Vec3 random_in_hemisphere(const Vec3 &normal) {
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

#endif //RAY_TRACING_IN_CPP_VEC3_H
