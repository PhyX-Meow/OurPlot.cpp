#include "../include/our_plot.h"
#include <string>

canvas_2d::canvas_2d(int height, int width, range x_, range y_) {
    data = img2d(height, imgrow(width, White));
    x = x_;
    y = y_;
    step_x = x_.length() / width;
    step_y = y_.length() / height;
    origin.clm = i_floor(-x_.min / x_.length() * width);
    origin.row = i_floor(-y_.min / y_.length() * height);
}
point canvas_2d::aff_trans(point p) { //need repair
    return {p.x / step_x, p.y / step_y};
}
pix_pos canvas_2d::to_pix_noaff(point p) {
    return {i_floor(p.x) + origin.clm, i_floor(p.y) + origin.row};
}
pix_pos canvas_2d::to_pix(point p) {
    return {i_floor(p.x / step_x) + origin.clm, i_floor(p.y / step_y) + origin.row};
}
point canvas_2d::to_point(pix_pos pos) {
    return {(pos.clm - origin.clm + 0.5) * step_x, (pos.row - origin.row + 0.5) * step_y};
}
void canvas_2d::draw_line(point ini, point end, pix color) {
    //first step: affine trans.
    point start{aff_trans(ini)};
    point ending{aff_trans(end)};

    //second step: search responding pixel positions

    //last step: for every pixel position, paint 9*9 square

    /*
    pix_pos start = to_pix(ini), ending = to_pix(end);
    double slope = (end.y - ini.y) / (end.x - ini.x);
    double c;
    pix_pos centre;
    double dist;

    if (abs_d(slope) <= 1.0) {
        c = end.y - slope * end.x;
        if (ini.x > end.x)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.clm; i <= ending.clm; ++i) {
            double t = to_point({i, origin.row}).x;
            double h = slope * (t - ini.x) + ini.y;
            centre = to_pix({t, h});
            if (contains(centre))
                (*this)[centre] = color;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    //dist = (to_point({centre.clm + i, centre.row + j}).x * slope + c - to_point({centre.clm + i, centre.row + j}).y) * (to_point({centre.clm + i, centre.row + j}).x * slope + c - to_point({centre.clm + i, centre.row + j}).y) / (slope * slope + 1);
                }
            }
        }
    } else {
        slope = (end.x - ini.x) / (end.y - ini.y);
        c = end.x - end.y * slope;
        if (ini.y > end.y)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.row; i <= ending.row; ++i) {
            double t = to_point({i, origin.clm}).y;
            double h = slope * (t - ini.y) + ini.x;
            if (contains(to_pix({h, t})))
                (*this)[to_pix({h, t})] = color;
        }
    }*/
}
canvas_2d &operator<<(canvas_2d &target, func_1var shape) {
    shape.paint_to(target);
    return target;
}

void func_1var::paint_to(canvas_2d target) {
    pix color = get_color();
    double precis = get_precis();
    point ini{0, func(0)}, end{precis, func(precis)};
    while (target.contains(target.to_pix(end))) {
        target.draw_line(ini, end, color);
        ini = end;
        end.x += precis;
        end.y = func(end.x);
    }
    ini = {0, func(0)};
    end = {-precis, func(-precis)};
    while (target.contains(target.to_pix(end))) {
        target.draw_line(ini, end, color);
        ini = end;
        end.x -= precis;
        end.y = func(end.x);
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
