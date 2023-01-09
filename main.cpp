#include "util.h"

#include "Camera.h"
#include "Color.h"
#include "Hittable_list.h"
#include "Sphere.h"

#include <iostream>

using namespace std;

Color ray_color(const Ray &ray, const Hittable &world, int depth) {
    if (depth <= 0) {
        return {0, 0, 0};
    }

    if (Hit_record record; world.hit(ray, 0, infinity, record)) {
        Point3 target = record.point + record.normal + random_in_unit_sphere();
        return 0.5 * ray_color(Ray(record.point, target - record.point), world, depth - 1);
    }

    Vec3 unit_direction = unit_vector(ray.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const auto image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 100;
    const int max_depth = 50;

    // World
    Hittable_list world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera camera;

    // Render
    cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < sample_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray ray = camera.get_ray(u, v);
                pixel_color += ray_color(ray, world, max_depth);
            }
            write_color(cout, pixel_color, sample_per_pixel);
        }
    }

    cerr << "\nDone.\n";

    return 0;
}
