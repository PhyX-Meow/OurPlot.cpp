#include "../include/toy_3d.h"

float_pos toy_3d::projection(point_3d p) {
    double x_{p.x * x_proj[0] + p.y * x_proj[1] + p.z * x_proj[2]};
    double y_{p.x * y_proj[0] + p.y * y_proj[1] + p.z * y_proj[2]};
    return {x_, y_};
}

toy_3d &operator<<(toy_3d &target, line_3d L) {
    float_pos ini{target.projection(L.ini)}, end{target.projection(L.end)};
    target.draw_line(ini, end, L.color);
    return target;
}
toy_3d &operator<<(toy_3d &target, surface F) {
    double precis = F.precis;
    int mesh = F.mesh;
    for (double s = -8.0; s < 8.0; s += precis * mesh)
        for (double t = -8.0; t < 8.0; t += precis * mesh) {
        }
    return target;
}