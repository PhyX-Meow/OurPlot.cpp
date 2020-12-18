#include "../include/bmp_io.h"
#include "../include/plot_2d.h"

canvas_2d::canvas_2d(int width, int height, range x_, range y_) {
    data = img2d(height, pixrow(width, White));
    x = x_;
    y = y_;
    step_x = x_.length() / width;
    step_y = y_.length() / height;
    origin.clm = i_floor(-x_.min / x_.length() * width);
    origin.row = i_floor(-y_.min / y_.length() * height);
}

void canvas::draw_line(float_pos ini, float_pos end, pix color, plot_style style) {
    //first step: affine trans.
    pix_pos start = to_pix(ini), ending = to_pix(end);
    double slope, c, dist2;
    slope = (end.y - ini.y) / (end.x - ini.x);

    pix_pos centre;
    //second step: search responding pixel positions
    if (abs_d(slope) <= 1.0) {
        c = ini.y - slope * ini.x;
        if (ini.x > end.x)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.clm; i <= ending.clm; ++i) {
            double t = to_affine(pix_pos{i, origin.row}).x;
            double h = slope * t + c;
            centre = to_pix({t, h});

            //last step: for every pixel position, paint 9*9 square
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    float_pos aff = to_affine(centre.add(i, j));
                    dist2 = (aff.x * slope + c - aff.y) * (aff.x * slope + c - aff.y) / (slope * slope + 1);
                    if (contains(centre.add(i, j))) {
                        if (dist2 <= 0.5)
                            (*this)[centre.add(i, j)] = color;
                        else if (dist2 <= 1)
                            (*this)[centre.add(i, j)] = color * 0.5;
                    }
                }
        }
    } else {
        slope = (end.x - ini.x) / (end.y - ini.y);
        c = ini.x - ini.y * slope;
        if (ini.y > end.y)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.row; i <= ending.row; ++i) {
            double t = to_affine(pix_pos{origin.clm, i}).y;
            double h = slope * t + c;
            centre = to_pix({h, t});

            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    float_pos aff = to_affine(centre.add(i, j));
                    dist2 = (aff.y * slope + c - aff.x) * (aff.y * slope + c - aff.x) / (slope * slope + 1);
                    if (contains(centre.add(i, j))) {
                        if (dist2 <= 0.5)
                            (*this)[centre.add(i, j)] = color;
                        else if (dist2 <= 1)
                            (*this)[centre.add(i, j)] = color * 0.5;
                    }
                }
        }
    }
}

canvas_2d &operator<<(canvas_2d &target, line L) {
    float_pos ini{target.to_affine(L.ini)}, end{target.to_affine(L.end)};
    target.draw_line(ini, end, L.color);
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_1var curve) {
    pix color = curve.color;
    double precis = curve.precis;
    for (double t = target.x.min - target.step_x; t < target.x.max + target.step_x; t += precis)
        target << line({t, curve.func(t)}, {t + precis, curve.func(t + precis)}, color);
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_polar curve) {
    pix color = curve.color;
    double precis = curve.precis;
    double init_t = curve.angle.min, end_t = curve.angle.max;
    double r, r_next = curve.func(init_t);
    for (double theta = init_t; theta < end_t + precis; theta += precis) {
        r = r_next;
        r_next = curve.func(theta + precis);
        precis = r > 1 ? curve.precis / r : curve.precis;
        target << line(polar(r, theta), polar(r_next, theta + precis), color);
    }
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_para curve) {
    pix color = curve.color;
    double precis = curve.precis;
    for (double t = curve.time.min; t < curve.time.max + precis; t += precis)
        target << line({curve.func_x(t), curve.func_y(t)}, {curve.func_x(t + precis), curve.func_y(t + precis)}, color);
    return target;
}