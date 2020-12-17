#ifndef _OUR_PLOT_
#define _OUR_PLOT_

#include "plot_basic.h"
#include <cmath>

// abs : double -> double
inline double abs_d(double x) {
    return x > 0 ? x : -x;
}
// i_floor : double -> int
inline int i_floor(double x) {
    return x >= 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}

struct range {
    double min;
    double max;
    double length() {
        return max - min;
    }
};
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

struct pix_pos {
    int clm;
    int row;
    pix_pos add(pix_pos adder) {
        return {clm + adder.clm, row + adder.row};
    }
    pix_pos add(int n, int m) {
        return {clm + n, row + m};
    }
};

enum plot_style {
    thin,
    medium,
    thick
};

class canvas_2d {
  public:
    img2d data;
    pix &operator[](const pix_pos &pos) {
        return data[pos.row][pos.clm];
    }
    canvas_2d(int width, int height, range x_, range y_);

    int height() { return data.size(); }
    int width() { return data[0].size(); }

    bool contains(pix_pos pos) {
        return (pos.clm < width()) && (pos.clm >= 0) && (pos.row < height()) && (pos.row >= 0);
    }
    void draw_line(point ini_, point end_, pix color, plot_style style = thin);
    int save_as(const char filename[]);

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
    func_1var(double (*func_)(double), pix color_) {
        func = func_;
        color = color_;
    }
    func_1var(double (*func_)(double), pix color_, double precis_) {
        func = func_;
        color = color_;
        precis = precis_;
    }

    pix color;
    double (*func)(double);
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_1var shape);

class func_polar {
  public:
    func_polar(double (*func_)(double), pix color_) {
        func = func_;
        color = color_;
    }
    func_polar(double (*func_)(double), pix color_, double precis_) {
        func = func_;
        color = color_;
        precis = precis_;
    }
    pix color;
    double (*func)(double);
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_polar shape);

class func_para {
  public:
    func_para(double (*func_x_)(double), double (*func_y_)(double), range t_, pix color_) {
        func_x = func_x_;
        func_y = func_y_;
        t = t_;
        color = color_;
    }
    func_para(double (*func_x_)(double), double (*func_y_)(double), range t_, pix color_, double precis_) {
        func_x = func_x_;
        func_y = func_y_;
        t = t_;
        color = color_;
        precis = precis_;
    }

    pix color;
    range t;
    double (*func_x)(double);
    double (*func_y)(double);
    double precis{0.1};
};
canvas_2d &operator<<(canvas_2d &target, func_para shape);

class axes {
};

#endif