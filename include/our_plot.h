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

class canvas_2d;
class func_1var {
};

class func_polar {
};

class func_para {
};

class axes {
};