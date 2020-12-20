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

double capsuleSDF(float_pos pt, float_pos ini, float_pos end, double r) {
    vector_2d ini_pt{pt.x - ini.x, pt.y - ini.y}, ini_end{end.x - ini.x, end.y - ini.y};
    vector_2d proj_pt{ini_pt.proj_to(ini_end, 0.0, 1.0)};
    vector_2d dist{ini_pt - proj_pt};
    double c{sqrt(dist * dist) - r};
    return std::max(std::min(0.5 - c, 1.0), 0.0);
}
/*
pix alphablend(pix p, pix color_new, double alpha) {
    p.r = (unsigned char) (p.r * (1 - alpha) + color_new.r * alpha * 255);
    p.g = (unsigned char) (p.g * (1 - alpha) + color_new.g * alpha * 255);
    p.b = (unsigned char) (p.b * (1 - alpha) + color_new.b * alpha * 255);
    return p;
}*/
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
                (*this)[{x, y}] = (*this)[{x, y}].min(color * (capsuleSDF(aff, ini, end, r)));
            }
        }
    }
}

/*

void canvas::draw_line(float_pos ini, float_pos end, pix color, plot_style style) {
    //first step: affine trans.
    double r{1};
    pix_pos start = to_pix(ini), ending = to_pix(end);
    double slope, c;
    slope = (end.y - ini.y) / (end.x - ini.x);

    pix_pos centre;
    //second step: search responding pixel positions
    if (std::abs(slope) <= 1.0) {
        c = ini.y - slope * ini.x;
        if (ini.x > end.x)
            std::swap(ini, end), std::swap(start, ending);
        for (int i = start.clm; i <= ending.clm; ++i) {
            double t = to_affine(pix_pos{i, origin.row}).x;
            double h = slope * t + c;
            centre = to_pix({t, h});

            //last step: for every pixel position, paint 9*9 square
            for (int i = -5; i <= 5; i++)
                for (int j = -5; j <= 5; j++) {
                    float_pos aff = to_affine(centre.add(i, j));
                    //dist2 = (aff.x * slope + c - aff.y) * (aff.x * slope + c - aff.y) / (slope * slope + 1);
                    if (contains(centre.add(i, j))) {
                        (*this)[centre.add(i, j)] = (*this)[centre.add(i, j)].min(color * (capsuleSDF(aff, ini, end, r)));
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

            for (int i = -5; i <= 5; i++)
                for (int j = -5; j <= 5; j++) {
                    float_pos aff = to_affine(centre.add(i, j));
                    //dist2 = (aff.y * slope + c - aff.x) * (aff.y * slope + c - aff.x) / (slope * slope + 1);
                    if (contains(centre.add(i, j))) {
                        (*this)[centre.add(i, j)] = (*this)[centre.add(i, j)].min(color * (capsuleSDF(aff, ini, end, r)));
                    }
                }
        }
    }
}
*/
canvas_2d &operator<<(canvas_2d &target, line L) {
    float_pos ini{target.to_affine(L.ini)}, end{target.to_affine(L.end)};
    target.draw_line(ini, end, L.color);
    return target;
}
canvas_2d &operator<<(canvas_2d &target, func_1var curve) {
    pix color = curve.color;
    double precis = curve.precis;
    double d;
    for (double t = target.x.min - target.step_x; t < target.x.max + target.step_x; t += precis) {
        d = (curve.func(t + (1e-5)) - curve.func(t)) / (1e-5);
        precis = d > 1 ? curve.precis / d : curve.precis;
        precis = std::max(precis, target.step_x / 8);
        if (std::abs(curve.func(t + precis) - curve.func(t)) / precis > (double) target.height() / 3) continue;
        target << line({t, curve.func(t)}, {t + precis, curve.func(t + precis)}, color);
    }
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

void canvas_2d::draw_axes() {
    float_pos left{0.5 - origin.clm, 0}, right{width() - 0.5 - origin.clm, 0}, up{0, height() - 0.5 - origin.row}, down{0, 0.5 - origin.row};

    if (origin.clm >= 0 && origin.clm <= width())
        draw_line(left, right, Black, thin);
    if (origin.row >= 0 && origin.row <= height())
        draw_line(down, up, Black, thin);

    double tick_height{4.0}, tick_ratio{15.0};
    double tick_x{get_tick(x.length(), tick_ratio) / step_x}, tick_y{get_tick(y.length(), tick_ratio) / step_y};
    double digit_x{get_digits(x.length(), tick_ratio)}, digit_y{get_digits(y.length(), tick_ratio)};

    draw_number(origin, 0, 0, O);
    if (origin.clm <= width())
        for (double i = tick_x; i < right.x; i += tick_x) {
            draw_line({i, 0}, {i, tick_height}, Black);
            draw_number(to_pix({i, 0}), i * step_x, digit_x, X);
        }
    if (origin.clm > 0)
        for (double i = -tick_x; i > left.x; i -= tick_x) {
            draw_line({i, 0}, {i, tick_height}, Black);
            draw_number(to_pix({i, 0}), i * step_x, digit_x, X);
        }
    if (origin.row <= width())
        for (double i = tick_y; i < up.y; i += tick_y) {
            draw_line({0, i}, {tick_height, i}, Black);
            draw_number(to_pix({0, i}), i * step_y, digit_y, Y);
        }
    if (origin.row > 0)
        for (double i = -tick_y; i > down.y; i -= tick_y) {
            draw_line({0, i}, {tick_height, i}, Black);
            draw_number(to_pix({0, i}), i * step_y, digit_y, Y);
        }

    draw_line({right.x - 8, 8}, right, Black);
    draw_line({right.x - 8, -8}, right, Black);
    draw_line({8, up.y - 8}, up, Black);
    draw_line({-8, up.y - 8}, up, Black);
};
