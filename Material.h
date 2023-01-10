#ifndef RAY_TRACING_IN_CPP_MATERIAL_H
#define RAY_TRACING_IN_CPP_MATERIAL_H

#include <utility>

#include "util.h"
#include "Hittable.h"
#include "Texture.h"

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
    std::shared_ptr<Texture> albedo;

    Vec3 (*scatter_direction_function)(const Vec3 &);

    explicit Diffuse(const Color &color) : albedo(std::make_shared<Solid_color>(color)) {
        scatter_direction_function = lambertian;
    }

    explicit Diffuse(shared_ptr<Texture> texture): albedo(std::move(texture)) {
        scatter_direction_function = lambertian;
    }

    Diffuse(const Color &color, DiffuseType type) : albedo(std::make_shared<Solid_color>(color)) {
        switch (type) {
            using enum DiffuseType;
            case Simple: {
                scatter_direction_function = simple;
                break;
            }
            case Alternate: {
                scatter_direction_function = alternate;
                break;
            }
            case TrueLambertian: {
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

        scattered = Ray(record.point, scatter_direction, ray_in.time());
        attenuation = albedo->value(record.u, record.v, record.point);
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
        scattered = Ray(rec.point, reflected + fuzziness * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

class Dielectric : public Material {
public:
    double _index_of_refraction; // Index of Refraction

    explicit Dielectric(double index_of_refraction) : _index_of_refraction(index_of_refraction) {}

    bool scatter(const Ray &r_in, const Hit_record &rec, Color &attenuation, Ray &scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / _index_of_refraction) : _index_of_refraction;

        Vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double() ) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = Ray(rec.point, direction, r_in.time());
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow(1 - cosine, 5);
    }

};

#endif //RAY_TRACING_IN_CPP_MATERIAL_H
