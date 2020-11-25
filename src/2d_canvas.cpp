#include "../include/our_plot.h"

class canvas_2d {
    std::vector<std::vector<pix>> data;
    int height();
    int width();
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    void draw_line(poi x, poi y);
};