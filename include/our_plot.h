#include "bmp_io.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

struct poi {
    double x;
    double y;
    poi euclid(double x_, double y_);
    poi polar(double r, double th);
};

class canvas_2d {
    std::vector<std::vector<pix>> data;
    void modify_color(double x, double y, pix color);
};

class line {
    poi x;
    poi y;
    pix color;
    canvas_2d &operator<<(canvas_2d &canvas);
};

class func_1var {
};

class func_polar {
};

class func_para {
};

class axes {
};