#include "../include/plot_3d.h"

canvas_3d::canvas_3d(int width, int height, range x, range y, range z, point_3d viewpoint) {
}

float_pos canvas_3d::proj(point_3d p) {
    return {0, 0};
}
canvas_3d &operator<<(canvas_3d &target, line_3d L) {
    float_pos ini{target.proj(L.ini)}, end{target.proj(L.end)};
    target.draw_line(ini, end, L.color);
    return target;
}