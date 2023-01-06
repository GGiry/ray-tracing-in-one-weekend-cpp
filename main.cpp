#include "Color.h"
#include "Vec3.h"

#include <iostream>

using namespace std;

int main() {
    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render
    cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
            write_color(cout, pixel_color);
        }
    }

    return 0;
}
