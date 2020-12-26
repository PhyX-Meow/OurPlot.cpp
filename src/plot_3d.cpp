#include "../include/plot_3d.h"

canvas_3d::canvas_3d(int width, int height, range x, range y, range z, point_3d viewpoint) {
}

float_pos canvas_3d::projection(point_3d p) {
}
canvas_3d &operator<<(canvas_3d &target, line_3d L) {
    float_pos ini{target.projection(L.ini)}, end{target.projection(L.end)};
    target.draw_line(ini, end, L.color);
    return target;
}