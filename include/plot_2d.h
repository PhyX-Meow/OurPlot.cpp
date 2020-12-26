#ifndef _PLOT_2D_
#define _PLOT_2D_

#include "plot_basic.h"

struct point {
    double x;
    double y;
    point() {}
    point(double x_, double y_) {
        x = x_;
        y = y_;
    }
    inline point operator+(point adder) {
        return {x + adder.x, y + adder.y};
    }
};

inline point euclid(double x, double y) {
    return point(x, y);
}

inline point polar(double r, double th) {
    return point(r * cos(th), r * sin(th));
}

enum axe_type {
    X,
    Y,
    O
};

class canvas_2d : public canvas {
  public:
    canvas_2d(int width, int height, range x_, range y_);

    inline float_pos to_affine(point p) {
        return {p.x / step_x, p.y / step_y};
    }

    double step_x;
    double step_y;
    void draw_axes();
    void draw_number(pix_pos base, double number, int f_digits, axe_type type);

    range x;
    range y;
};

class line {
  public:
    line(point ini_, point end_, pix color_ = Black) {
        ini = ini_;
        end = end_;
        color = color_;
    }
    point ini;
    point end;
    pix color;
};
canvas_2d &operator<<(canvas_2d &target, line L);

class func_1var {
  public:
    func_1var(real_func func_, pix color_, double precis_ = 0.1, axe_type var_ = X) {
        func = func_;
        color = color_;
        precis = precis_;
        var = var_;
    }
    inline double operator()(double x) {
        return func(x);
    }

    real_func func;
    pix color;
    double precis{0.1};
    axe_type var;
};
canvas_2d &operator<<(canvas_2d &target, func_1var shape);

class func_polar {
  public:
    func_polar(real_func func_, range angle_, pix color_, double precis_ = 0.1) {
        func = func_;
        color = color_;
        angle = angle_;
        precis = precis_;
    }
    inline double operator()(double th) {
        return func(th);
    }

    real_func func;
    range angle;
    pix color;
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_polar shape);

class func_para {
  public:
    func_para(real_func func_x_, real_func func_y_, range time_, pix color_, double precis_ = 0.1) {
        func_x = func_x_;
        func_y = func_y_;
        time = time_;
        color = color_;
        precis = precis_;
    }
    inline point operator()(double t) {
        return euclid(func_x(t), func_y(t));
    }

    real_func func_x, func_y;
    range time;
    pix color;
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_para shape);

#endif