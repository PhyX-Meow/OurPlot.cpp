#include "./include/our_plot.h"

int main() {
    canvas_2d c(1000, 1000, {-1.5, 1.5}, {-1.5, 1.5});
    func_1var f([](double x) { return asin(std::abs(x) - 1) - pi / 2; }, Red, thin, 0.01);
    func_polar heart([](double t) { return 1 - sin(t); }, {0, 2 * pi}, Red, thin);
    func_polar flower([](double t) { return sin(1.3 * t); }, {0, 20 * pi}, Red, thick, 0.01);
    func_para circle([](double x) { return point{cos(x), sin(x)}; }, {0, 2 * pi}, Black, thin);
    c.draw_axes(15.0);
    c << flower;
    c.save_as("demo.bmp");

    return 0;
}