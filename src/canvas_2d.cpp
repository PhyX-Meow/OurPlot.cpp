#include "../include/our_plot.h"
#include <string>

canvas_2d::canvas_2d(int height, int width, range x_, range y_) {
    data = img2d(height, imgrow(width, White));
    x = x_;
    y = y_;
    step_x = x_.length() / width;
    step_y = y_.length() / height;
    origin.clm = static_cast<int>(x_.min / x_.length() * width);
    origin.row = static_cast<int>(y_.min / y_.length() * height);
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
        if (ini.x > end.x)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.clm; i <= ending.clm; ++i) {
            double t = to_point({origin.row, i}).x;
            double h = slope * (t - ini.x) + ini.y;
            if (range_check(to_pix({t, h})))
                (*this)[to_pix({t, h})] = color;
        }
    } else {
        slope = (end.x - ini.x) / (end.y - ini.y);
        if (ini.y > end.y)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.row; i <= ending.row; ++i) {
            double t = to_point({i, origin.clm}).y;
            double h = slope * (t - ini.y) + ini.x;
            if (range_check(to_pix({h, t})))
                (*this)[to_pix({h, t})] = color;
        }
    }
}
void func_1var::draw(canvas_2d target_canvas) {
    point ini{0, func(0)}, end{precis, func(precis)};
    while (target_canvas.range_check(target_canvas.to_pix(end))) {
        target_canvas.draw_line(ini, end, color);
        ini = end;
        end.x += precis;
        end.y = func(end.x);
    }
    ini = {0, func(0)};
    end = {-precis, func(-precis)};
    while (target_canvas.range_check(target_canvas.to_pix(end))) {
        target_canvas.draw_line(ini, end, color);
        ini = end;
        end.x -= precis;
        end.y = func(end.x);
    }
}
void func_para::draw(canvas_2d target_canvas) {
    double para{precis};
    int cnt{0};
    point ini{func_x(0), func_y(0)}, end{func_x(precis), func_y(precis)};
    while (target_canvas.range_check(target_canvas.to_pix(end)) && cnt <= end_cnt) {
        target_canvas.draw_line(ini, end, color);
        ini = end;
        para += precis;
        end = {func_x(para), func_y(para)};
        cnt++;
    }
    ini = {func_x(0), func_y(0)};
    end = {func_x(-precis), func_y(-precis)};
    para = -precis;
    cnt = 0;
    while (target_canvas.range_check(target_canvas.to_pix(end)) && cnt <= end_cnt) {
        target_canvas.draw_line(ini, end, color);
        ini = end;
        para -= precis;
        end = {func_x(para), func_y(para)};
        cnt++;
    }
}
