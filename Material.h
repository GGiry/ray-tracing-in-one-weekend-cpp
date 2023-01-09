#ifndef RAY_TRACING_IN_CPP_MATERIAL_H
#define RAY_TRACING_IN_CPP_MATERIAL_H

#include "util.h"
#include "Hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray_in, const Hit_record &record, Color &attenuation, Ray &scattered) const = 0;
};


#endif //RAY_TRACING_IN_CPP_MATERIAL_H
