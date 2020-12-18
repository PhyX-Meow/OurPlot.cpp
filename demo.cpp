#include "./include/our_plot.h"

int main() {
    canvas_2d c(800, 800, {-2, 2}, {-3, 1});
    func_1var f([](double x) { return sin(x); });
    func_polar heart([](double t) { return 1 - sin(t); }, {0, 2 * pi}, Red);
    func_para circle([](double x) { return cos(x); }, [](double x) { return sin(x); }, {0, 2 * pi});
    c << heart << circle << line({-4, 0}, {4, 0}) << line({0, -4}, {0, 4});
    c.save_as("demo.bmp");

    return 0;
}