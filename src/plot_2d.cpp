#include "../include/plot_2d.h"

pix RGB_f(double red, double green, double blue) {
    col r = static_cast<col>(red * 255);
    col g = static_cast<col>(green * 255);
    col b = static_cast<col>(blue * 255);
    return {r, g, b};
}
pix HSV(double hue, double s, double v) {
    int h = std::floor(hue / 60.0);
    double f = hue / 60.0 - h;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);
    if (h == 1)
        return RGB_f(q, v, p);
    else if (h == 2)
        return RGB_f(p, v, t);
    else if (h == 3)
        return RGB_f(p, q, v);
    else if (h == 4)
        return RGB_f(t, p, v);
    else if (h == 5)
        return RGB_f(v, p, q);
    else
        return RGB_f(v, t, p);
}

double capsuleSDF(float_pos pt, float_pos ini, float_pos end, double r) {
    vector_2d ini_pt{pt.x - ini.x, pt.y - ini.y}, ini_end{end.x - ini.x, end.y - ini.y};
    vector_2d proj_pt{ini_pt.proj_to(ini_end, 0.0, 1.0)};
    vector_2d dist{ini_pt - proj_pt};
    double c{sqrt(dist * dist) - r};
    return std::max(std::min(0.5 - c, 1.0), 0.0);
}
// pix alphablend(pix p, pix color_new, double alpha) {
//     p.r = (unsigned char) (p.r * (1 - alpha) + color_new.r * alpha * 255);
//     p.g = (unsigned char) (p.g * (1 - alpha) + color_new.g * alpha * 255);
//     p.b = (unsigned char) (p.b * (1 - alpha) + color_new.b * alpha * 255);
//     return p;
// }

void canvas::draw_line(float_pos ini, float_pos end, pix color, plot_style style) {
    double r;
    if (style == thin)
        r = 0.5;
    if (style == medium)
        r = 1;
    if (style == thick)
        r = 1.5;
    pix_pos start = to_pix({std::min(ini.x, end.x) - r, std::min(ini.y, end.y) - r});
    pix_pos ending = to_pix({std::max(ini.x, end.x) + r, std::max(ini.y, end.y) + r});
    for (int x = start.clm; x <= ending.clm; ++x) {
        for (int y = start.row; y <= ending.row; ++y) {
            float_pos aff = to_affine({x, y});
            if (contains({x, y})) {
                (*this)[{x, y}] = (*this)[{x, y}].min(color * capsuleSDF(aff, ini, end, r));
            }
        }
    }
}

// void canvas::draw_line(float_pos ini, float_pos end, pix color, plot_style style) {
//     //first step: affine trans.
//     double r{1};
//     pix_pos start = to_pix(ini), ending = to_pix(end);
//     double slope, c;
//     slope = (end.y - ini.y) / (end.x - ini.x);

//     pix_pos centre;
//     //second step: search responding pixel positions
//     if (std::abs(slope) <= 1.0) {
//         c = ini.y - slope * ini.x;
//         if (ini.x > end.x)
//             std::swap(ini, end), std::swap(start, ending);
//         for (int i = start.clm; i <= ending.clm; ++i) {
//             double t = to_affine(pix_pos{i, origin.row}).x;
//             double h = slope * t + c;
//             centre = to_pix({t, h});

//             //last step: for every pixel position, paint 9*9 square
//             for (int i = -5; i <= 5; i++)
//                 for (int j = -5; j <= 5; j++) {
//                     float_pos aff = to_affine(centre.add(i, j));
//                     //dist2 = (aff.x * slope + c - aff.y) * (aff.x * slope + c - aff.y) / (slope * slope + 1);
//                     if (contains(centre.add(i, j))) {
//                         (*this)[centre.add(i, j)] = (*this)[centre.add(i, j)].min(color * (capsuleSDF(aff, ini, end, r)));
//                     }
//                 }
//         }
//     } else {
//         slope = (end.x - ini.x) / (end.y - ini.y);
//         c = ini.x - ini.y * slope;
//         if (ini.y > end.y)
//             std::swap(ini, end), std::swap(start, ending);
//         for (int i = start.row; i <= ending.row; ++i) {
//             double t = to_affine(pix_pos{origin.clm, i}).y;
//             double h = slope * t + c;
//             centre = to_pix({h, t});

//             for (int i = -5; i <= 5; i++)
//                 for (int j = -5; j <= 5; j++) {
//                     float_pos aff = to_affine(centre.add(i, j));
//                     //dist2 = (aff.y * slope + c - aff.x) * (aff.y * slope + c - aff.x) / (slope * slope + 1);
//                     if (contains(centre.add(i, j))) {
//                         (*this)[centre.add(i, j)] = (*this)[centre.add(i, j)].min(color * (capsuleSDF(aff, ini, end, r)));
//                     }
//                 }
//         }
//     }
// }

bool in_trig(float_pos P, float_pos A, float_pos B, float_pos C) {
    vector_2d a{A - P};
    vector_2d b{B - P};
    vector_2d c{C - P};
    double ab{cross(a, b)}, bc{cross(b, c)}, ca{cross(c, a)};
    if ((ab < 0 && bc > 0) || (ab > 0 && bc < 0))
        return false;
    if ((ca < 0 && bc > 0) || (ca > 0 && bc < 0))
        return false;
    return true;
}

void canvas::paint_trig(float_pos A, float_pos B, float_pos C, pix color) {
    if (A.y > B.y)
        std::swap(A, B);
    if (B.y > C.y)
        std::swap(B, C);
    if (A.y > B.y)
        std::swap(A, B);
    int y_min = to_pix(A).row, y_max = to_pix(C).row;
    if (A.x > B.x)
        std::swap(A, B);
    if (B.x > C.x)
        std::swap(B, C);
    if (A.x > B.x)
        std::swap(A, B);
    int x_min = to_pix(A).clm, x_max = to_pix(C).clm;
    for (int i = x_min; i <= x_max; ++i)
        for (int j = y_min; j <= y_max; ++j)
            if (in_trig(to_affine({i, j}), A, B, C))
                (*this)[{i, j}] = color;
}

canvas_2d::canvas_2d(int width, int height, range x_, range y_) {
    data = img2d(height, pixrow(width, White));
    x = x_;
    y = y_;
    step_x = x_.length() / width;
    step_y = y_.length() / height;
    origin.clm = i_floor(-x_.min / x_.length() * width);
    origin.row = i_floor(-y_.min / y_.length() * height);
}

canvas_2d &operator<<(canvas_2d &target, line L) {
    float_pos ini{target.to_affine(L.ini)}, end{target.to_affine(L.end)};
    target.draw_line(ini, end, L.color, L.style);
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_1var f) {
    pix color = f.color;
    double precis = f.precis;
    double d;
    if (f.var == X)
        for (double t = target.x.min - target.step_x; t < target.x.max + target.step_x; t += precis) {
            d = (f(t + (1e-5)) - f(t)) / (1e-5);
            precis = d > 1 ? f.precis / d : f.precis;
            precis = std::max(precis, target.step_x / 8);
            if (std::abs(f(t + precis) - f(t)) / precis > (double) target.height() / 3) continue;
            target << line({t, f(t)}, {t + precis, f(t + precis)}, color, f.style);
        }
    else
        for (double t = target.y.min - target.step_y; t < target.x.max + target.step_y; t += precis) {
            d = (f(t + (1e-5)) - f(t)) / (1e-5);
            precis = d > 1 ? f.precis / d : f.precis;
            precis = std::max(precis, target.step_y / 8);
            if (std::abs(f(t + precis) - f(t)) / precis > (double) target.height() / 3) continue;
            target << line({f(t), t}, {f(t + precis), t + precis}, color, f.style);
        }
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_polar r) {
    pix color = r.color;
    double precis = r.precis;
    double init_t = r.angle.min, end_t = r.angle.max;
    double r0, r1 = r(init_t);
    for (double theta = init_t; theta < end_t + precis; theta += precis) {
        r0 = r1;
        r1 = r(theta + precis);
        precis = r0 > 1 ? r.precis / r0 : r.precis;
        target << line(polar(r0, theta), polar(r1, theta + precis), color, r.style);
    }
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_para path) {
    pix color = path.color;
    double precis = path.precis;
    for (double t = path.time.min; t < path.time.max + precis; t += precis)
        target << line(path(t), path(t + precis), color, path.style);
    return target;
}

double get_tick(double length, double tick_ratio) {
    length /= tick_ratio;
    double tmp = log10(length);
    int pow = i_floor(tmp);
    tmp -= pow;
    double ans{1.0};
    if (pow >= 0)
        for (int i{0}; i < pow; ++i)
            ans *= 10.0;
    else
        for (int i{0}; i > pow; --i)
            ans /= 10.0;
    if (log10(2.0) < tmp && tmp < log10(5.0))
        ans *= 2.0;
    else if (log10(5.0) < tmp)
        ans *= 5.0;
    return ans;
}

double get_digits(double length, double tick_ratio) {
    length /= tick_ratio;
    return std::max(-i_floor(log10(length)), 0);
}

void canvas_2d::draw_number(pix_pos base, double tick, int digits, axe_type type) {
    int digit_width{12}, digit_height{14}, dot_width{4}, minus_width{9};

    if (type == O) {
        pen = base.minus(digit_width + 5, digit_height + 5);
        attach("./img/0_20pt.bmp");
        return;
    }

    bool dot{digits > 0 ? true : false}, minus{tick < 0 ? true : false};
    tick = std::abs(tick);
    int num = i_floor(tick);
    tick -= num;
    std::string prefix{fmt::to_string(num)}, suffix;
    for (int i = 0; i < digits; ++i) {
        tick *= 10.0;
        num = i_floor(tick);
        suffix.push_back('0' + num);
        tick -= num;
    }

    int len = prefix.length() + suffix.length();
    if (type == X) {
        pen = base.minus(digit_width / 2 * len, digit_height + 5);
        if (minus) pen.clm -= minus_width * 2 / 3;
        if (dot) pen.clm -= dot_width / 2;
    } else {
        pen = base.minus(digit_width * len + 5, digit_height / 2);
        if (minus) pen.clm -= minus_width;
        if (dot) pen.clm -= dot_width;
    }

    if (minus) {
        attach("./img/minus_20pt.bmp");
        pen.clm += minus_width;
    }
    for (auto ch : prefix) {
        attach(fmt::format("./img/{}_20pt.bmp", ch));
        pen.clm += digit_width;
    }

    if (dot) {
        attach("./img/dot_20pt.bmp");
        pen.clm += dot_width;
        for (auto ch : suffix) {
            attach(fmt::format("./img/{}_20pt.bmp", ch));
            pen.clm += digit_width;
        }
    }
}

void canvas_2d::draw_axes(double tick_ratio) {
    float_pos left{0.5 - origin.clm, 0}, right{width() - 0.5 - origin.clm, 0}, up{0, height() - 0.5 - origin.row}, down{0, 0.5 - origin.row};

    if (origin.clm >= 0 && origin.clm <= width())
        draw_line(left, right, Black, medium);
    if (origin.row >= 0 && origin.row <= height())
        draw_line(down, up, Black, medium);

    double tick_height{5.0};
    double tick_x{get_tick(x.length(), tick_ratio) / step_x}, tick_y{get_tick(y.length(), tick_ratio) / step_y};
    double digit_x{get_digits(x.length(), tick_ratio)}, digit_y{get_digits(y.length(), tick_ratio)};

    draw_number(origin, 0, 0, O);
    if (origin.clm <= width())
        for (double i = tick_x; i < right.x - 10; i += tick_x) {
            draw_line({i, 0}, {i, tick_height}, Black);
            draw_number(to_pix({i, 0}), i * step_x, digit_x, X);
        }
    if (origin.clm > 0)
        for (double i = -tick_x; i > left.x + 10; i -= tick_x) {
            draw_line({i, 0}, {i, tick_height}, Black);
            draw_number(to_pix({i, 0}), i * step_x, digit_x, X);
        }
    if (origin.row <= width())
        for (double i = tick_y; i < up.y - 10; i += tick_y) {
            draw_line({0, i}, {tick_height, i}, Black);
            draw_number(to_pix({0, i}), i * step_y, digit_y, Y);
        }
    if (origin.row > 0)
        for (double i = -tick_y; i > down.y + 10; i -= tick_y) {
            draw_line({0, i}, {tick_height, i}, Black);
            draw_number(to_pix({0, i}), i * step_y, digit_y, Y);
        }

    draw_line({right.x - 8, 8}, right, Black);
    draw_line({right.x - 8, -8}, right, Black);
    draw_line({8, up.y - 8}, up, Black);
    draw_line({-8, up.y - 8}, up, Black);
};