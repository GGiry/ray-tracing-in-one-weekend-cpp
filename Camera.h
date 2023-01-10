#ifndef RAY_TRACING_IN_CPP_CAMERA_H
#define RAY_TRACING_IN_CPP_CAMERA_H

#include "util.h"

class Camera {
public:
    Point3 origin = Point3(0, 0, 0);
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera(
            Point3 look_from,
            Point3 look_at,
            Vec3 view_up,
            double vertical_field_of_view,    // Vertical field-of-view in degree
            double aspect_ratio
    ) : origin(look_from) {
        auto theta = degrees_to_radians(vertical_field_of_view);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(look_from - look_at);
        auto u = unit_vector(cross(view_up, w));
        auto v = cross(w, u);

        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    [[nodiscard]] Ray get_ray(double s, double t) const {
        return {origin, lower_left_corner + s * horizontal + t * vertical - origin};
    }
};

#endif //RAY_TRACING_IN_CPP_CAMERA_H
