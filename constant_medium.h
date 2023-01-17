#ifndef RAY_TRACING_IN_CPP_CONSTANT_MEDIUM_H
#define RAY_TRACING_IN_CPP_CONSTANT_MEDIUM_H

#include <utility>

#include "util.h"

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class Constant_medium : public Hittable {
public:
    shared_ptr<Hittable> boundary;
    shared_ptr<Material> phase_function;
    double neg_inv_density;

    Constant_medium(shared_ptr<Hittable> _boundary, double density, const shared_ptr<Texture> &texture)
            : boundary(std::move(_boundary)),
              phase_function(make_shared<Isotropic>(texture)),
              neg_inv_density(-1 / density) {}

    Constant_medium(shared_ptr<Hittable> _boundary, double density, Color color)
            : boundary(std::move(_boundary)),
              phase_function(make_shared<Isotropic>(color)),
              neg_inv_density(-1 / density) {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const override;

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        return boundary->bounding_box(time0, time1, output_box);
    }
};

bool Constant_medium::hit(const Ray &ray, double t_min, double t_max, Hit_record &rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    Hit_record rec1;
    Hit_record rec2;

    if (!boundary->hit(ray, -infinity, infinity, rec1)) { return false; }

    if (!boundary->hit(ray, rec1.t + 0.0001, infinity, rec2)) { return false; }

    if (debugging) { std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n'; }

    if (rec1.t < t_min) { rec1.t = t_min; }
    if (rec2.t > t_max) { rec2.t = t_max; }

    if (rec1.t >= rec2.t) { return false; }

    if (rec1.t < 0) { rec1.t = 0; }

    const auto ray_length = ray.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.point = ray.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
                  << "rec.t = " << rec.t << '\n'
                  << "rec.p = " << rec.point << '\n';
    }

    rec.normal = Vec3(1, 0, 0); // arbitrary
    rec.front_face = true;  // also arbitrary
    rec.material_ptr = phase_function;

    return true;
}

#endif //RAY_TRACING_IN_CPP_CONSTANT_MEDIUM_H
