#ifndef _TOY_PLOT_3D_
#define _TOY_PLOT_3D_

#include "plot_3d.h"

const double x_proj[]{24.8438, -16.175, 0.0};
const double y_proj[]{-5.375, -5.3, 145.0};

class toy_3d : public canvas {
  public:
    toy_3d(int width, int height, double scale_ = 1.0) {
        data = img2d(height, pixrow(width, White));
        origin.clm = width / 2;
        origin.row = height / 2;
        scale = scale_;
    }
    float_pos proj(point_3d p);

    double scale;
};
toy_3d &operator<<(toy_3d &target, line_3d L);
toy_3d &operator<<(toy_3d &target, surface F);
toy_3d &operator<<(toy_3d &target, curve c);

#endif