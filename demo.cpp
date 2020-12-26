#include "./include/our_plot.h"

int main() {
    canvas_2d c(1000, 1000, {-4, 4}, {-4, 4});
    func_1var f([](double x) { return asin(std::abs(x) - 1) - pi / 2; }, Red, 0.01);
    func_polar heart([](double t) { return 1 - sin(t); }, {0, 2 * pi}, Red);
    func_polar flower([](double t) { return sin(1.3 * t); }, {0, 20 * pi}, Red, 0.01);
    func_para circle([](double x) { return cos(x); }, [](double x) { return sin(x); }, {0, 2 * pi}, Black);
    c.draw_axes();
    c << f;
    c.save_as("demo.bmp");

    return 0;
}