#ifndef RAY_TRACING_IN_CPP_CAMERA_H
#define RAY_TRACING_IN_CPP_CAMERA_H

#include "util.h"

class Camera {
public:
    Point3 origin = Point3(0, 0, 0);
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u;
    Vec3 v;
    Vec3 w;
    double lens_radius;
    double shutter_open_time;
    double shutter_close_time;

    Camera(
            Point3 look_from,
            Point3 look_at,
            Vec3 view_up,
            double vertical_field_of_view,    // Vertical field-of-view in degree
            double aspect_ratio,
            double aperture,
            double focus_distance,
            double open_time = 0,
            double close_time = 0
    ) : origin(look_from), lens_radius(aperture / 2), shutter_open_time(open_time), shutter_close_time(close_time) {
        auto theta = degrees_to_radians(vertical_field_of_view);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);

        horizontal = focus_distance * viewport_width * u;
        vertical = focus_distance * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_distance * w;
    }

    [[nodiscard]] Ray get_ray(double s, double t) const {
        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = u * rd.x() + v * rd.y();
        return {origin + offset,
                lower_left_corner + s * horizontal + t * vertical - origin - offset,
                random_double(shutter_open_time, shutter_close_time)};
    }
};

#endif //RAY_TRACING_IN_CPP_CAMERA_H
