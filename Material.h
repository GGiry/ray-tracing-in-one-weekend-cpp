#ifndef RAY_TRACING_IN_CPP_MATERIAL_H
#define RAY_TRACING_IN_CPP_MATERIAL_H

#include "util.h"
#include "Hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const = 0;
};

enum class DiffuseType {
    Simple, TrueLambertian, Alternate
};


class Diffuse : public Material {
public:
    Color albedo;

    Vec3 (*scatter_direction_function)(const Vec3 &);

    explicit Diffuse(const Color &color) : albedo(color) {
        scatter_direction_function = lambertian;
    }

    Diffuse(const Color &color, DiffuseType type) : albedo(color) {
        switch (type) {
            case DiffuseType::Simple: {
                scatter_direction_function = simple;
                break;
            }
            case DiffuseType::Alternate: {
                scatter_direction_function = alternate;
                break;
            }
            case DiffuseType::TrueLambertian: {
                scatter_direction_function = lambertian;
                break;
            }
        }
    }

    bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const override {
        auto scatter_direction = scatter_direction_function(record.normal);

        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:

    static Vec3 simple(const Vec3 &normal) {
        return normal + random_in_unit_sphere();
    }

    static Vec3 lambertian(const Vec3 &normal) {
        return normal + random_unit_vector();
    }

    static Vec3 alternate(const Vec3 &normal) {
        return random_in_hemisphere(normal);
    }
};


class Metal : public Material {
public:
    Color albedo;
    double fuzziness;

    Metal(const Color &a, double fuzz) : albedo(a), fuzziness(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray &r_in, const Hit_record &rec, Color &attenuation, Ray &scattered) const override {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.point, reflected + fuzziness * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

#endif //RAY_TRACING_IN_CPP_MATERIAL_H
