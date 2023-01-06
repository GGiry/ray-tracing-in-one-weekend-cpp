#ifndef RAY_TRACING_IN_CPP_COLOR_H
#define RAY_TRACING_IN_CPP_COLOR_H


#include "Vec3.h"

#include <iostream>

void write_color(std::ostream &out, Color pixel_color) {
    // Write the translated [0, 255] value for each component.

    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}


#endif //RAY_TRACING_IN_CPP_COLOR_H
