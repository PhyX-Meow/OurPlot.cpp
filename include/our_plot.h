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
};

class canvas_2d {
  public:
    img2d data;
    pix &operator[](const pix_pos &pos) {
        return data[pos.row][pos.clm];
    }
    canvas_2d(int height, int width, range x_, range y_);
    int height() {
        return data.size();
    }
    int width() {
        return data[0].size();
    }
    bool contains(pix_pos pos) {
        return (pos.clm < width()) && (pos.clm >= 0) && (pos.row < height()) && (pos.row >= 0);
    }
    void draw_line(point ini_, point end_, pix color);

    pix_pos to_pix(point p);
    point to_affine(point p);
    point to_affine(pix_pos pos);

  private:
    range x;
    range y;
    pix_pos origin;
    double step_x;
    double step_y;
};

class func_1var {
  public:
    func_1var(double (*func_)(double), pix color_) {
        func = func_;
        set_color(color_);
    }
    func_1var(double (*func_)(double), pix color_, double precis_) {
        func = func_;
        set_color(color_);
        set_precis(precis_);
    }
    void set_color(pix color_) {
        color = color_;
    }
    pix get_color() {
        return color;
    }
    void set_precis(double precis_) {
        max_precis = precis_;
    }
    double get_precis() {
        return max_precis;
    }
    void paint_to(canvas_2d target);

  private:
    pix color;
    double (*func)(double);
    int max_precis;
};
canvas_2d &operator<<(canvas_2d &target, func_1var shape);

class func_polar {
};

class func_para {
  public: //end_cnt is just for test;
    func_para(double (*func_x_)(double), double (*func_y_)(double), double precis_, pix color_, int end_cnt_) {
        func_x = func_x_;
        func_y = func_y_;
        precis = precis_;
        color = color_;
        end_cnt = end_cnt_;
    }
    void paint_to(canvas_2d target);

  private:
    double (*func_x)(double);
    double (*func_y)(double);
    double precis;
    pix color;
    int end_cnt;
};

class axes {
};

#endif