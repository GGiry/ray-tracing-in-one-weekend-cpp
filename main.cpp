#include "Color.h"
#include "Ray.h"
#include "Vec3.h"

#include <iostream>

using namespace std;

double hit_sphere(const Point3 &center, double radius, const Ray &ray) {
    Vec3 origin_center = ray.origin() - center;
    auto a = ray.direction().length_squared();
    auto half_b = dot(origin_center, ray.direction());
    auto c = origin_center.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

Color ray_color(const Ray &ray) {
    auto t = hit_sphere(Point3(0, 0, -1), 0.5, ray);

    if (t > 0.0) {
        Vec3 N = unit_vector(ray.at(t) - Vec3(0, 0, -1));
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    Vec3 unit_direction = unit_vector(ray.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const auto image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewport_width, 0, 0);
    auto vertical = Vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

    // Render
    cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);

            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(ray);
            write_color(cout, pixel_color);
        }
    }

    cerr << "\nDone.\n";

    return 0;
}
