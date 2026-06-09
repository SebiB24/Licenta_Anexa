//
// Created by sebyc on 26/02/2026.
//

#ifndef IMAGE_FILE_COLOR_H
#define IMAGE_FILE_COLOR_H

#include "interval.h"

using namespace std;

using color = vec3;

//gamma space is the space that apeals more to the human visual perception
//ower ayes are more perceptive to differences between darker colors that
//between brighter ones so a 50% reflection appears closer to
//10% reflectin (the darker color) then to 90% (the brighter one)
inline double linear_to_gamma(double linear_val){
    if(linear_val > 0)
        return std::sqrt(linear_val);
    return 0;
}

void write_color(ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out<<rbyte << " " << gbyte << " " << bbyte << "\n";

};

#endif //IMAGE_FILE_COLOR_H
