#include "./include/our_plot.h"

int main() {
    canvas_2d task_1(1000, 1000, {-8, 8}, {-8, 8});
    task_1.draw_axes();
    task_1 << line({-10, -10}, {10, 10}, pix(0x246224), medium);
    task_1.save_as("task_1.bmp");

    canvas_2d task_2(1000, 1000, {-6, 6}, {-1, 11});
    task_2.draw_axes();
    task_2 << func_1var([](double x) { return x * x; }, pix(0x567EAF), medium, 0.01);
    task_2.save_as("task_2.bmp");

    canvas_2d task_3(1000, 1000, {-2, 2}, {-3, 1});
    task_3 << func_polar([](double th) { return 1 - sin(th); }, {0, 2 * pi}, pix(0x66CCFF), medium, 0.01);
    task_3.save_as("task_3.bmp");

    canvas_2d task_4(1000, 1000, {-3, 3}, {-4, 2});
    task_4.draw_axes();
    task_4 << func_para([](double t) { return euclid(1 + cos(t), sin(t)); }, {0, pi}, Red, thick, 0.01)
           << func_para([](double t) { return euclid(-1 + cos(t), sin(t)); }, {0, pi}, Red, thick, 0.01)
           << func_para([](double t) { return euclid(1 + cos(t), -t); }, {0, pi}, pix(0xFB7A2D), thick, 0.01)
           << func_para([](double t) { return euclid(-1 - cos(t), -t); }, {0, pi}, pix(0xFB7A2D), thick, 0.01);
    task_4.save_as("task_4.bmp");

    canvas_2d task_5(1000, 1000, {-8, 8}, {-8, 8});
    task_5.draw_axes();
    task_5 << func_para([](double t) { return euclid(1 + cos(t), sin(t)); }, {0, pi}, Red, thick, 0.01)
           << func_para([](double t) { return euclid(-1 + cos(t), sin(t)); }, {0, pi}, Red, thick, 0.01)
           << func_para([](double t) { return euclid(1 + cos(t), -t); }, {0, pi}, pix(0xFB7A2D), thick, 0.01)
           << func_para([](double t) { return euclid(-1 - cos(t), -t); }, {0, pi}, pix(0xFB7A2D), thick, 0.01)
           << func_para([](double t) { return euclid(5 * cos(t) - 5 * sin(t) / sqrt(3), 5 * cos(t) + 5 * sin(t) / sqrt(3)); }, {-(2.0 / 3.0) * pi, pi / 3.0}, Black, medium, 0.01)
           << func_para([](double t) { return euclid(-5 * cos(t) + 5 * sin(t) / sqrt(3), 5 * cos(t) + 5 * sin(t) / sqrt(3)); }, {-(2.0 / 3.0) * pi, pi / 3.0}, Black, medium, 0.01);
    task_5.save_as("task_5.bmp");

    toy_3d task_6(1200, 900, 1.5);
    task_6 << surface(
        [](double x, double y) {double r = sqrt(x * x + y * y); if (r < 1e-5) return 1.0; return sin(r) / r; },
        [](point_3d p) { return HSV((1.0 - p.z) * 240.0, 1.0, 1.0); },
        0.1);
    task_6.save_as("task_6.bmp");

    canvas_2d add_1(1000, 1000, {-2 * pi, 2 * pi}, {-6, 6});
    add_1.draw_axes();
    add_1 << func_1var([](double x) { return tan(x); }, Blue, medium);
    add_1.save_as("add_1.bmp");

    canvas_2d add_2(1000, 1000, {-1.5, 1.5}, {-1.5, 1.5});
    add_2.draw_axes();
    add_2 << func_polar([](double th) { return sin(1.3 * th); }, {0, 20 * pi}, Red, thin, 0.01);
    add_2.save_as("add_2.bmp");

    canvas_2d add_3(1000, 1000, {-6, 6}, {-2, 2});
    add_3.draw_axes();
    add_3 << func_1var([](double x) { return sin(x * x); }, Black, thin, 0.01);
    add_3.save_as("add_3.bmp");

    return 0;
}