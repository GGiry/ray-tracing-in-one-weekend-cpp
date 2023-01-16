#include "util.h"

#include "aa_rectangle.h"
#include "bvh.h"
#include "Camera.h"
#include "Color.h"
#include "Hittable_list.h"
#include "Sphere.h"
#include "Material.h"
#include "Moving_sphere.h"

#include <iostream>
#include <algorithm>

#include <mutex>
#include <future>


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

Hittable_list simple_light() {
    Hittable_list objects;

    auto perlin_texture = make_shared<Noise_texture>(4);
    objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Diffuse>(perlin_texture)));
    objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Diffuse>(perlin_texture)));

    auto diffuse_light = make_shared<Diffuse_light>(Color(4, 4, 4));
    objects.add(make_shared<xy_rectangle>(3, 5, 1, 3, -2, diffuse_light));

    return objects;
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

struct Ray_result {
    unsigned int index = 0;
    Color color = Color(0, 0, 0);
};

struct Scene {
    Camera camera;
    Color background;
    unique_ptr<Hittable> world;
};

Scene choose_scene(int id) {
    Scene scene;

    switch (id) {
        case 1:
            scene.world = make_unique<BVH_node>(random_scene(), 0, 1);
            scene.background = Color(0.70, 0.80, 1.00);
            scene.camera = Camera(Point3(13, 2, 3), Point3(0, 0, 0), Vec3(0, 1, 0), 20, 16. / 9., 0.1, 10., 0, 1);
            break;

        case 2:
            scene.world = make_unique<BVH_node>(two_spheres(), 0, 1);
            scene.background = Color(0.70, 0.80, 1.00);
            scene.camera = Camera(Point3(13, 2, 3), Point3(0, 0, 0), Vec3(0, 1, 0), 20, 16. / 9., 0.1, 10., 0, 1);
            break;

        case 3:
            scene.world = make_unique<BVH_node>(two_perlin_spheres(), 0, 1);
            scene.background = Color(0.70, 0.80, 1.00);
            scene.camera = Camera(Point3(13, 2, 3), Point3(0, 0, 0), Vec3(0, 1, 0), 20, 16. / 9., 0.1, 10., 0, 1);
            break;

        case 4:
            scene.world = make_unique<BVH_node>(earth(), 0, 1);
            scene.background = Color(0.70, 0.80, 1.00);
            scene.camera = Camera(Point3(13, 2, 3), Point3(0, 0, 0), Vec3(0, 1, 0), 20, 16. / 9., 0.1, 10., 0, 1);

            break;

        default:
            // case 5:
            scene.world = make_unique<BVH_node>(simple_light(), 0, 1);
            scene.background = Color(0.0, 0.0, 0.0);
            scene.camera = Camera(Point3(26, 3, 6), Point3(0, 2, 0), Vec3(0, 1, 0), 20, 16. / 9., 0.1, 10., 0, 1);
            break;
    }

    return scene;
}

class Image {
public:
    double aspect_ration = 0.0;
    int width = 0;
    int height = 0;
    int sample_per_pixel = 0;
    int max_depth = 0;

    Image() = default;

    Image(double _aspect_ratio, int _width, int _sample_per_pixel, int _max_depth)
            : aspect_ration(_aspect_ratio), width(_width), height(static_cast<int>(_width / _aspect_ratio)),
              sample_per_pixel(_sample_per_pixel), max_depth(_max_depth) {}
};


void create_jobs(const Image &image, const Scene &scene, mutex &mutex, vector<std::future<Ray_result>> &m_futures) {
    for (int j = image.height - 1; j >= 0; --j) {
        for (int i = 0; i < image.width; ++i) {
            auto future = std::async(
                    launch::async | launch::deferred,
                    [&scene, image, i, j]() {
                        const unsigned int index = (image.height - 1 - j) * image.width + i;
                        Color pixel_color(0, 0, 0);

                        for (int s = 0; s < image.sample_per_pixel; ++s) {
                            auto u = (i + random_double()) / (image.width - 1);
                            auto v = (j + random_double()) / (image.height - 1);

                            Ray ray = scene.camera.get_ray(u, v);
                            pixel_color += ray_color(ray, scene.background, *scene.world,
                                                     image.max_depth);
                        }
                        pixel_color /= float(image.sample_per_pixel);

                        Ray_result result;
                        result.index = index;
                        result.color = Color(sqrt(pixel_color[0]), sqrt(pixel_color[1]),
                                             sqrt(pixel_color[2]));
                        return result;
                    });

            {
                lock_guard lock(mutex);
                m_futures.push_back(std::move(future));
            }
        }
    }
}

int main() {
    // Image
    Image image = {16.0 / 9.0, 800, 400, 50};

    // World
    Scene scene = choose_scene(0);

    cerr << "image_width: " << image.width << endl;
    cerr << "image_height: " << image.height << endl;

    // Compute
    std::mutex mutex;
    std::condition_variable cvResults;
    std::vector<std::future<Ray_result>> m_futures;

    create_jobs(image, scene, mutex, m_futures);

    const int pixel_count = image.width * image.height;

    cerr << "Pixel count: " << pixel_count << endl;

    cerr << "\nWait until jobs are done.\n";
    {
        unique_lock lock(mutex);
        cvResults.wait(lock, [&m_futures, &pixel_count] {
            return m_futures.size() == pixel_count;
        });
    }

    cerr << "\nComputation Done.\n";

    auto pixels = vector<Color>(pixel_count, Color(0, 0, 0));

    // reconstruct image.
    auto done_count = 0;
    auto last_percentage = 0;
    auto current_percentage = 0;
    for (std::future<Ray_result> &ray_result: m_futures) {
        Ray_result result = ray_result.get();
        done_count++;

        current_percentage = int(double(done_count) / double(pixel_count) * 100.0);

        if (current_percentage != last_percentage) {
            cerr << "\rreconstruct: " << current_percentage << ' ' << flush;
            last_percentage = current_percentage;
        }

        pixels[result.index] = result.color;
    }

    cerr << "\nReconstruction Done.\n";

    // Output image
    cout << "P3\n" << image.width << ' ' << image.height << "\n255\n";

    for (unsigned int i = 0; i < pixel_count; ++i) {
        write_color(cout, pixels[i]);
    }

    return 0;
}
