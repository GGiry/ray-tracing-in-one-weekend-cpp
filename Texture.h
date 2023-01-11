#ifndef RAY_TRACING_IN_CPP_TEXTURE_H
#define RAY_TRACING_IN_CPP_TEXTURE_H

#include <utility>

#include "util.h"
#include "perlin.h"

class Texture {
public:
    [[nodiscard]] virtual Color value(double u, double v, const Point3 &point) const = 0;

    virtual ~Texture() = default;
};

class Solid_color : public Texture {
public:
    Solid_color() = default;

    explicit Solid_color(Color color) : color_value(color) {}

    Solid_color(double red, double green, double blue) : Solid_color(Color(red, green, blue)) {}

    [[nodiscard]] Color value(double u, double v, const Point3 &point) const override {
        return color_value;
    }

private:
    Color color_value;
};

class Checker_texture : public Texture {
public:
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;

    Checker_texture() = default;

    Checker_texture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd) : even(std::move(_even)),
                                                                                     odd(std::move(_odd)) {}

    Checker_texture(Color _even, Color _odd) : even(std::make_shared<Solid_color>(_even)),
                                               odd(std::make_shared<Solid_color>(_odd)) {}

    [[nodiscard]] Color value(double u, double v, const Point3 &point) const override {

        if (auto sines = sin(10 * point.x()) * sin(10 * point.y()) * sin(10 * point.z()); sines < 0) {
            return odd->value(u, v, point);
        }
        return even->value(u, v, point);
    }
};

class Noise_texture : public Texture {
public:
    Perlin noise;
    double scale = 1.0;

    Noise_texture() = default;

    explicit Noise_texture(double _scale) : scale(_scale) {}


    [[nodiscard]] Color value(double u, double v, const Point3 &point) const override {
        return Color(1, 1, 1) * noise.turbulence(scale * point);
    }
};

#endif //RAY_TRACING_IN_CPP_TEXTURE_H
