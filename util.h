#ifndef RAY_TRACING_IN_CPP_UTIL_H
#define RAY_TRACING_IN_CPP_UTIL_H

#include <cmath>
#include <limits>
#include <memory>
#include <numbers>

// Common Headers
#include "Ray.h"
#include "Vec3.h"

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

#endif //RAY_TRACING_IN_CPP_UTIL_H
