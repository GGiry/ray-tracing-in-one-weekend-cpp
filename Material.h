#ifndef RAY_TRACING_IN_CPP_MATERIAL_H
#define RAY_TRACING_IN_CPP_MATERIAL_H

#include "util.h"
#include "Hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const = 0;
};

class SimpleDiffuse : public Material {
public:
    Color albedo;

    explicit SimpleDiffuse(const Color &color) : albedo(color) {}

    bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const override {
        auto scatter_direction = record.normal + random_in_unit_sphere();

        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class TrueLambertian : public Material {
public:
    Color albedo;

    explicit TrueLambertian(const Color &color) : albedo(color) {}

    bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const override {
        auto scatter_direction = record.normal + random_unit_vector();

        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class AlternateDiffuse : public Material {
public:
    Color albedo;

    explicit AlternateDiffuse(const Color &color) : albedo(color) {}

    bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const override {
        auto scatter_direction = random_in_hemisphere(record.normal);

        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material {
public:
    Color albedo;

    explicit Metal(const Color &a) : albedo(a) {}

    bool scatter(const Ray &r_in, const Hit_record &rec, Color &attenuation, Ray &scattered) const override {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.point, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

#endif //RAY_TRACING_IN_CPP_MATERIAL_H
