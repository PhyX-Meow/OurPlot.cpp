#ifndef _PLOT_3D_
#define _PLOT_3D_

#include "plot_basic.h"

struct point_3d {
    double x;
    double y;
    double z;
};

class line_3d {
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
    canvas_3d(range x, range y, range z, point_3d viewpoint);
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