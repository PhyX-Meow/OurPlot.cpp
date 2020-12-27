#include "../include/toy_3d.h"

float_pos toy_3d::proj(point_3d p) {
    double x_{p.x * x_proj[0] + p.y * x_proj[1] + p.z * x_proj[2]};
    double y_{p.x * y_proj[0] + p.y * y_proj[1] + p.z * y_proj[2]};
    return {scale * x_, scale * y_};
}

toy_3d &operator<<(toy_3d &target, line_3d L) {
    float_pos ini{target.proj(L.ini)}, end{target.proj(L.end)};
    target.draw_line(ini, end, L.color);
    return target;
}
toy_3d &operator<<(toy_3d &target, surface F) {
    double precis = F.precis;
    int mesh = F.mesh;
    double step = precis * mesh;
    double x{}, y{};
    point_3d now{}, next{};
    float_pos mm, mM, Mm, MM;
    for (double s = -8.0; s < 8.0; s += step) {
        for (double t = -8.0; t < 8.0; t += step) {
            x = s;
            for (int i = 0; i < mesh; ++i) {
                y = t;
                for (int j = 0; j < mesh; ++j) {
                    mm = target.proj({x, y, F(x, y)});
                    mM = target.proj({x, y + precis, F(x, y + precis)});
                    Mm = target.proj({x + precis, y, F(x + precis, y)});
                    MM = target.proj({x + precis, y + precis, F(x + precis, y + precis)});
                    target.paint_trig(mm, mM, Mm, White);
                    target.paint_trig(mM, Mm, MM, White);
                    if (i == 0) {
                        now = {x, y, F(x, y)};
                        next = {x, y + precis, F(x, y + precis)};
                        target << line_3d(now, next, F.color(now));
                    }
                    if (j == 0) {
                        now = {x, y, F(x, y)};
                        next = {x + precis, y, F(x + precis, y)};
                        target << line_3d(now, next, F.color(now));
                    }
                    y += precis;
                }
                x += precis;
            }
            // x = s;
            // y = t;
            // next = {x, y, F(x, y)};
            // for (int i = 0; i < mesh; ++i) {
            //     now = next;
            //     next = {x + precis, y, F(x + precis, y)};
            //     target << line_3d(now, next, F.color(now));
            //     x += precis;
            // }
            // x = s;
            // next = {x, y, F(x, y)};
            // for (int i = 0; i < mesh; ++i) {
            //     now = next;
            //     next = {x, y + precis, F(x, y + precis)};
            //     target << line_3d(now, next, F.color(now));
            //     y += precis;
            // }
        }
    }

    // x += step;
    next = {x, -8.0, F(x, -8.0)};
    for (double t = -8.0; t < 8.0; t += precis) {
        now = next;
        next = {x, t + precis, F(x, t + precis)};
        target << line_3d(now, next, F.color(now));
    }
    next = {-8.0, y, F(-8.0, y)};
    for (double s = -8.0; s < 8.0; s += precis) {
        now = next;
        next = {s + precis, y, F(s + precis, y)};
        target << line_3d(now, next, F.color(now));
    }

    return target;
}