#ifndef _PLOT_3D_
#define _PLOT_3D_

#include "plot_basic.h"

struct point_3d {
    double x;
    double y;
    double z;
};
struct vector_3d {
    double x;
    double y;
    double z;
    vector_3d operator+(vector_3d &v) {
        return {(*this).x + v.x, (*this).y + v.y, (*this).y + v.y};
    }
    double operator*(vector_3d &v) {
        return (*this).x * v.x + (*this).y * v.y + (*this).z * v.z;
    }
};

class line_3d {
  public:
    line_3d(point_3d ini_, point_3d end_, pix color_ = Black) {
        ini = ini_;
        end = end_;
        color = color_;
    }

    point_3d ini;
    point_3d end;
    pix color;
};

class canvas_3d : public canvas {
  public:
    canvas_3d(int width, int height, range x, range y, range z, point_3d viewpoint);
    float_pos proj(point_3d p);
    void draw_axes();
    void draw_number();
};
canvas_3d &operator<<(canvas_3d &target, line_3d L);

using real_2var_func = std::function<double(double, double)>;
using color_func = std::function<pix(point_3d)>;

class surface {
  public:
    surface(real_2var_func func_, color_func color_, double precis_ = 0.1) {
        func = func_;
        color = color_;
        precis = precis_;
        mesh = i_floor(0.5 / precis_);
    }
    inline double operator()(double x, double y) {
        return func(x, y);
    }

    real_2var_func func;
    color_func color;
    double precis{0.1};
    int mesh{5};
};
canvas_3d &operator<<(canvas_3d &target, surface Sigma);

class curve {
  public:
    real_func func_x, func_y, func_z;
    pix color;
    double precis{0.1};
};
canvas_3d &operator<<(canvas_3d &target, curve Gamma);

class surface_para {
};

#endif