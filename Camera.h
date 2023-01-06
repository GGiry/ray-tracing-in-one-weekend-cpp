#ifndef RAY_TRACING_IN_CPP_CAMERA_H
#define RAY_TRACING_IN_CPP_CAMERA_H

#include "util.h"

class Camera {
public:
    Point3 origin = Point3(0, 0, 0);
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera() {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        horizontal = Vec3(viewport_width, 0.0, 0.0);
        vertical = Vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
    }

    [[nodiscard]] Ray get_ray(double u, double v) const {
        return {origin, lower_left_corner + u * horizontal + v * vertical - origin};
    }
};

#endif //RAY_TRACING_IN_CPP_CAMERA_H
