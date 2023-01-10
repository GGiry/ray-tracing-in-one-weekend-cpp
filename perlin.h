#ifndef RAY_TRACING_IN_CPP_PERLIN_H
#define RAY_TRACING_IN_CPP_PERLIN_H

#include "util.h"

class Perlin {
public:
    Perlin() {
        random_floats = std::vector<double>(point_count);
        for (int i = 0; i < point_count; ++i) {
            random_floats[i] = random_double();
        }

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    [[nodiscard]] double noise(const Point3 &point) const {
        auto i = static_cast<int>(4 * point.x()) & 255;
        auto j = static_cast<int>(4 * point.y()) & 255;
        auto k = static_cast<int>(4 * point.z()) & 255;

        return random_floats[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }

private:
    static const int point_count = 256;
    std::vector<double> random_floats;
    std::vector<int> perm_x;
    std::vector<int> perm_y;
    std::vector<int> perm_z;

    static std::vector<int> perlin_generate_perm() {
        auto result = std::vector<int>(point_count);

        for (int i = 0; i < point_count; ++i) {
            result[i] = i;
        }

        permute(result);

        return result;
    }

    static void permute(std::vector<int> p) {
        for (int i = point_count - 1; i > 0; --i) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }
};


#endif //RAY_TRACING_IN_CPP_PERLIN_H
