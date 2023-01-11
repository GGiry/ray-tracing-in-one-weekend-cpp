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
        auto u = point.x() - floor(point.x());
        auto v = point.y() - floor(point.y());
        auto w = point.z() - floor(point.z());

        auto i = static_cast<int>(floor(point.x()));
        auto j = static_cast<int>(floor(point.y()));
        auto k = static_cast<int>(floor(point.z()));

        vector<vector<vector<double>>> c{{{0, 0}, {0, 0}},
                                         {{0, 0}, {0, 0}}};

        for (int di = 0; di < 2; di++) {
            for (int dj = 0; dj < 2; dj++) {
                for (int dk = 0; dk < 2; dk++) {
                    c[di][dj][dk] = random_floats[
                            perm_x[(i + di) & 255] ^
                            perm_y[(j + dj) & 255] ^
                            perm_z[(k + dk) & 255]
                    ];
                }
            }
        }

        return trilinear_interp(c, u, v, w);
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

    static double trilinear_interp(vector<vector<vector<double>>> c, double u, double v, double w) {
        auto accum = 0.0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                    accum += (i * u + (1 - i) * (1 - u)) *
                             (j * v + (1 - j) * (1 - v)) *
                             (k * w + (1 - k) * (1 - w)) * c[i][j][k];

        return accum;
    }
};

#endif //RAY_TRACING_IN_CPP_PERLIN_H
