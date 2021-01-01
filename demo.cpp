#include "./include/our_plot.h"
#include "include/plot_2d.h"

int main() {
    canvas_2d task_1(1000, 1000, {-8, 8}, {-8, 8});
    task_1.draw_axes();
    task_1 << line({-10, -10}, {10, 10}, pix(0x246224), medium);
    task_1.save_as("task_1.bmp");

    return 0;
}