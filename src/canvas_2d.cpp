#include "../include/our_plot.h"
#include <string>

canvas_2d::canvas_2d(int height, int width, range x_, range y_) {
    data = img2d(height, imgrow(width, White));
    x = x_;
    y = y_;
    step_x = x_.length() / width;
    step_y = y_.length() / height;
    origin.clm = static_cast<int>(1.0 - x_.min / x_.length() * width);
    origin.row = static_cast<int>(1.0 - y_.min / y_.length() * height);
}

pix_pos canvas_2d::to_pix(point p) {
    return {static_cast<int>(p.y / step_y) + origin.row, static_cast<int>(p.x / step_x) + origin.clm};
}
point canvas_2d::to_point(pix_pos pos) {
    return {(pos.clm - origin.clm + 0.5) * step_x, (pos.row - origin.row + 0.5) * step_y};
}
void canvas_2d::draw_line(point ini, point end, pix color) {
    pix_pos start = to_pix(ini), ending = to_pix(end);
    double slope = (end.y - ini.y) / (end.x - ini.x);
    if (abs_d(slope) < 1) {
        for (int i = start.clm; i <= ending.clm; ++i) {
            double t = to_point({i, origin.row}).x;
            double h = slope * (t - ini.x) + ini.y;
            (*this)[to_pix({t, h})] = color;
        }
    }
}