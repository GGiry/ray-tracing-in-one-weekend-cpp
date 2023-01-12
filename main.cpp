#include "util.h"

#include "bvh.h"
#include "Camera.h"
#include "Color.h"
#include "Hittable_list.h"
#include "Sphere.h"
#include "Material.h"
#include "Moving_sphere.h"

#include <iostream>


using namespace std;

shared_ptr<Material> select_material(double choose_mat) {
    shared_ptr<Material> sphere_material;

    if (choose_mat < 0.8) {
        // diffuse
        auto albedo = Color::random() * Color::random();
        sphere_material = make_shared<Diffuse>(albedo);
    } else if (choose_mat < 0.95) {
        // metal
        auto albedo = Color::random(0.5, 1);
        auto fuzz = random_double(0, 0.5);
        sphere_material = make_shared<Metal>(albedo, fuzz);
    } else {
        // glass
        sphere_material = make_shared<Dielectric>(1.5);
    }

    return sphere_material;
}

shared_ptr<Hittable>
create_object(double choose_mat, const Point3 &center, const shared_ptr<Material> &sphere_material) {
    if (choose_mat < 0.8) {
        auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
        return make_shared<Moving_sphere>(center, center2, 0, 1, 0.2, sphere_material);
    } else {
        return make_shared<Sphere>(center, 0.2, sphere_material);
    }
}

Hittable_list two_spheres() {
    Hittable_list objects;

    auto checker = make_shared<Checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Diffuse>(checker)));
    objects.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Diffuse>(checker)));

    return objects;
}

Hittable_list two_perlin_spheres() {
    Hittable_list objects;

    auto perlin_texture = make_shared<Noise_texture>(4);

    objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Diffuse>(perlin_texture)));
    objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Diffuse>(perlin_texture)));

    return objects;
}

Hittable_list random_scene() {
    Hittable_list world;

    auto ground_material = make_shared<Checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Diffuse>(ground_material)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material = select_material(choose_mat);

                world.add(create_object(choose_mat, center, sphere_material));

            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Diffuse>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

Hittable_list earth() {
    auto earth_texture = make_shared<Image_texture>("earth-map.jpg");
    auto earth_surface = make_shared<Diffuse>(earth_texture);
    auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    return Hittable_list(globe);
}

Color ray_color(const Ray &ray, const Color &background_color, const Hittable &world, int depth) {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return {0, 0, 0};
    }

    Hit_record record;

    // If the ray hits nothing, return the background color.
    if (!world.hit(ray, 0.001, infinity, record))
        return background_color;

    Ray scattered;
    Color attenuation;
    Color emitted = record.material_ptr->emitted(record.u, record.v, record.point);

    if (!record.material_ptr->scatter(ray, record, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background_color, world, depth - 1);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int sample_per_pixel = 100;
    const int max_depth = 50;

    // World
    Point3 look_from;
    Point3 look_at;
    auto vertical_field_of_view = 40.0;
    auto aperture = 0.0;
    auto background_color = Color(0, 0, 0);

    Hittable_list world;

    switch (0) {
        case 1:
            world = random_scene();
            background_color = Color(0.70, 0.80, 1.00);
            look_from = Point3(13, 2, 3);
            look_at = Point3(0, 0, 0);
            vertical_field_of_view = 20;
            aperture = 0.1;
            break;

        case 2:
            world = two_spheres();
            background_color = Color(0.70, 0.80, 1.00);
            look_from = Point3(13, 2, 3);
            look_at = Point3(0, 0, 0);
            vertical_field_of_view = 20;
            break;

        case 3:
            world = two_perlin_spheres();
            background_color = Color(0.70, 0.80, 1.00);
            look_from = Point3(13, 2, 3);
            look_at = Point3(0, 0, 0);
            vertical_field_of_view = 20;
            break;

        case 4:
            world = earth();
            background_color = Color(0.70, 0.80, 1.00);
            look_from = Point3(13, 2, 3);
            look_at = Point3(0, 0, 0);
            vertical_field_of_view = 20.0;
            break;

        default:
            // case 5:
            world = earth();
            background_color = Color(0.0, 0.0, 0.0);
            look_from = Point3(13, 2, 3);
            look_at = Point3(0, 0, 0);
            break;
    }

    BVH_node bvh(world, 0, 1);

    // Camera
    const Vec3 view_up(0, 1, 0);
    auto distance_to_focus = 10.0;
    const auto image_height = static_cast<int>(image_width / aspect_ratio);

    Camera camera(look_from, look_at, view_up, vertical_field_of_view, aspect_ratio, aperture, distance_to_focus, 0, 1);

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
                pixel_color += ray_color(ray, background_color, bvh, max_depth);
            }
            write_color(cout, pixel_color, sample_per_pixel);
        }
    }

    cerr << "\nDone.\n";

    return 0;
}
