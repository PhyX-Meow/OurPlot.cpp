#include "../include/our_plot.h"
#include <string>

canvas_2d::canvas_2d(int height, int width, range x_, range y_) {
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
            double t = to_affine(pix_pos{i, origin.clm}).y;
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

canvas_2d &operator<<(canvas_2d &target, func_1var shape) {
    shape.paint_to(target);
    return target;
}

void func_1var::paint_to(canvas_2d &target) {
    pix color = get_color();
    double precis = get_precis();
    for (double t = target.x.min - target.step_x; t < target.x.max + target.step_x; t += precis) {
        target.draw_line({t, func(t)}, {t + precis, func(t + precis)}, color);
    }
}
void func_para::paint_to(canvas_2d target) {
    double para{precis};
    int cnt{0};
    point ini{func_x(0), func_y(0)}, end{func_x(precis), func_y(precis)};
    while (target.contains(target.to_pix(end)) && cnt <= end_cnt) {
        target.draw_line(ini, end, color);
        ini = end;
        para += precis;
        end = {func_x(para), func_y(para)};
        cnt++;
    }
    ini = {func_x(0), func_y(0)};
    end = {func_x(-precis), func_y(-precis)};
    para = -precis;
    cnt = 0;
    while (target.contains(target.to_pix(end)) && cnt <= end_cnt) {
        target.draw_line(ini, end, color);
        ini = end;
        para -= precis;
        end = {func_x(para), func_y(para)};
        cnt++;
    }
}