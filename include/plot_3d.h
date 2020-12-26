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
    /*vector_3d out_prod(vector_3d &v) {
        return ;
    }*/
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
    float_pos projection(point_3d p);
};
canvas_3d &operator<<(canvas_3d &target, line_3d L);

class surface {
};

class curve {
};

class surface_para {
};

class axes_3d {
};

#endif