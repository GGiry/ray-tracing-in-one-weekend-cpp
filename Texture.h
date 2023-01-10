#ifndef RAY_TRACING_IN_CPP_TEXTURE_H
#define RAY_TRACING_IN_CPP_TEXTURE_H

#include "util.h"

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


#endif //RAY_TRACING_IN_CPP_TEXTURE_H
