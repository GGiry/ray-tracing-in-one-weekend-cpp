#ifndef RAY_TRACING_IN_CPP_UTIL_H
#define RAY_TRACING_IN_CPP_UTIL_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <numbers>
#include <random>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = std::numbers::pi;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0, 1).
    static std::uniform_real_distribution distribution(0.0, 1.0);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

inline double random_double(double lower_bound, double upper_bound) {
    // Returns a random real in [min, max).
    return lower_bound + (upper_bound - lower_bound) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) { return min; }
    if (x > max) { return max; }
    return x;
}

// Common Headers
#include "Ray.h"
#include "Vec3.h"

#endif //RAY_TRACING_IN_CPP_UTIL_H
