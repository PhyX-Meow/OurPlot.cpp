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
    point operator+(point adder) {
        return {x + adder.x, y + adder.y};
    }
};

inline point euclid(double x, double y) {
    return point(x, y);
}

inline point polar(double r, double th) {
    return point(r * cos(th), r * sin(th));
}

using affine = point;

enum plot_style {
    thin,
    medium,
    thick
};

class canvas_2d : public canvas {
  public:
    canvas_2d(int width, int height, range x_, range y_);

    void draw_line(point ini_, point end_, pix color, plot_style style = thin);

    pix_pos to_pix(point p);
    point to_affine(point p);
    point to_affine(pix_pos pos);

    double step_x;
    double step_y;

    range x;
    range y;
    pix_pos origin;
};

class line {
  public:
    line(point ini_, point end_) {
        ini = ini_;
        end = end_;
    }
    line(point ini_, point end_, pix color_) {
        ini = ini_;
        end = end_;
        color = color_;
    }
    point ini;
    point end;
    pix color{Black};
};
canvas_2d &operator<<(canvas_2d &target, line L);

class func_1var {
  public:
    func_1var(double (*func_)(double), pix color_ = Blue, double precis_ = 0.1) {
        func = func_;
        color = color_;
        precis = precis_;
    }

    pix color;
    double (*func)(double);
    double precis;
};
canvas_2d &operator<<(canvas_2d &target, func_1var shape);

class func_polar {
  public:
    func_polar(double (*func_)(double), range angle_, pix color_ = Blue, double precis_ = 0.1) {
        func = func_;
        color = color_;
        angle = angle_;
        precis = precis_;
    }
    pix color;
    range angle;
    double (*func)(double);
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_polar shape);

class func_para {
  public:
    func_para(double (*func_x_)(double), double (*func_y_)(double), range time_, pix color_ = Blue, double precis_ = 0.1) {
        func_x = func_x_;
        func_y = func_y_;
        time = time_;
        color = color_;
        precis = precis_;
    }

    pix color;
    range time;
    double (*func_x)(double);
    double (*func_y)(double);
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_para shape);

class axes {
};

#endif