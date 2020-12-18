#include "../include/bmp_io.h"
#include "../include/our_plot.h"
#include <iostream>

canvas_2d::canvas_2d(int width, int height, range x_, range y_) {
    data = img2d(height, pixrow(width, White));
    x = x_;
    y = y_;
    step_x = x_.length() / width;
    step_y = y_.length() / height;
    origin.clm = i_floor(-x_.min / x_.length() * width);
    origin.row = i_floor(-y_.min / y_.length() * height);
}
point canvas_2d::to_affine(point p) {
    return {p.x / step_x, p.y / step_y};
}
point canvas_2d::to_affine(pix_pos pos) {
    return {pos.clm - origin.clm + 0.5, pos.row - origin.row + 0.5};
}
pix_pos canvas_2d::to_pix(point p) {
    return {i_floor(p.x) + origin.clm, i_floor(p.y) + origin.row};
}

void canvas_2d::draw_line(point ini_, point end_, pix color, plot_style style) {
    //first step: affine trans.
    affine ini = to_affine(ini_), end = to_affine(end_);
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
                    affine aff = to_affine(centre.add(i, j));
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
                    affine aff = to_affine(centre.add(i, j));
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
    target.draw_line(L.ini, L.end, L.color);
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_1var curve) {
    pix color = curve.color;
    double precis = curve.precis;
    for (double t = target.x.min - target.step_x; t < target.x.max + target.step_x; t += precis)
        target.draw_line({t, curve.func(t)}, {t + precis, curve.func(t + precis)}, color);
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
        target.draw_line(polar(r, theta), polar(r_next, theta + precis), color);
    }
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_para curve) {
    pix color = curve.color;
    double precis = curve.precis;
    for (double t = curve.time.min; t < curve.time.max + precis; t += precis)
        target.draw_line({curve.func_x(t), curve.func_y(t)}, {curve.func_x(t + precis), curve.func_y(t + precis)}, color);
    return target;
}
int canvas_2d::save_as(const char filename[]) {
    return bmp_write(data, filename);
}

//void canvas_2d::drawnumber(point start, int number, char o) {
//   int i = 0, j = 0, digits = 0, a[3];
//    double  = ;
//   pix_pos start1 = to_pix(start);
//    if (number <= 0) {
//        digits++;
//        number = -number;
//    }
//    while (number > 0) {
//        a[i] = number % 10;
//        i++;
//        number = number / 10;
//        digits++;
//    }
//    point shift = {0, 0};
//   if (o == 'x') {shift={-,-0.2};
//   }
//  if (o == 'y') {
//  }
//  if (o != 'x' && o != 'y') {
//      return;
//   }
//};

void canvas_2d::axes() {
    point o = to_affine(origin), left = {x.min / 1, 0}, right = {x.max / 1, 0}, up = {0, y.max / 1}, down = {0, y.min / 1}, little1 = {0, 0.1}, little2 = {0.1, 0}, fix = {-0.5, -0.5};
    point right1 = {x.max / 1 - 0.2, 0.2}, right2 = {x.max / 1 - 0.2, -0.2}, up1 = {-0.2, y.max / 1 - 0.2}, up2 = {0.2, y.max / 1 - 0.2};
    int i; //k, num1 = x.length() / 2, num2 = y.length() / 2;
    o = o + fix;
    draw_line(o + left, o + right, Black, thin);
    draw_line(o + down, o + up, Black, thin);
    draw_line(o + up, o + up1, Black, thin);
    draw_line(o + up, o + up2, Black, thin);
    draw_line(o + right, o + right1, Black, thin);
    draw_line(o + right, o + right2, Black, thin);
    point unitx = {1, 0}, unity = {0, 1};
    for (i = x.min / 1; i < x.max / 1 - 1; i++) {
        left = left + unitx;
        draw_line(o + left, o + left + little1, Black, thin);
        //    drawnumber(o + left, i, 'x');
    }
    for (i = y.min / 1; i < y.max / 1 - 1; i++) {
        down = down + unity;
        draw_line(o + down, o + down + little2, Black, thin);
        //    drawnumber(o + down, i, 'x');
    }
};
