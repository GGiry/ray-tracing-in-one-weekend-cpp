#ifndef RAY_TRACING_IN_CPP_TEXTURE_H
#define RAY_TRACING_IN_CPP_TEXTURE_H

#include <iostream>
#include <utility>

#include "util.h"
#include "rtw_stb_image.h"
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
        return Color(1, 1, 1) * 0.5 * (1 + sin(scale * point.z() + 10 * noise.turbulence(scale * point)));
    }
};

class Image_texture : public Texture {
private:
    unsigned char *data = nullptr;
    int width = 0;
    int height = 0;
    int bytes_per_scanline = 0;

public:
    const static int bytes_per_pixel = 3;

    Image_texture() = default;

    explicit Image_texture(const char *filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~Image_texture() override {
        delete data;
    }

    [[nodiscard]] Color value(double u, double v, const Vec3 &p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (data == nullptr)
            return {0, 1, 1};

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
    }

};

#endif //RAY_TRACING_IN_CPP_TEXTURE_H
