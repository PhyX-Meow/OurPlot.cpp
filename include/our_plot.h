#ifndef _OUR_PLOT_
#define _OUR_PLOT_

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

using col = unsigned char;
struct pix {
    col b;
    col g;
    col r;
    pix();
    pix(col red, col green, col blue) {
        r = red;
        g = green;
        b = blue;
    }
};

const pix White(0xFF, 0xFF, 0xFF);
const pix Red(0xFF, 0, 0);
const pix Green(0, 0xFF, 0);
const pix Blue(0, 0, 0xFF);

struct point {
    double x;
    double y;
    point();
    point(double x_, double y_) {
        x = x_;
        y = y_;
    }
};

inline point euclid(double x, double y) {
    return point(x, y);
}

inline point polar(double r, double th) {
    return point(r * cos(th), r * sin(th));
}

struct range {
    double min;
    double max;
};

class canvas_2d {
  public:
    std::vector<std::vector<pix>> data;
    range x;
    range y;
    canvas_2d(int height, int width, range x_, range y_) {
        data = std::vector<std::vector<pix>>(height, std::vector<pix>(width, White));
        x = x_;
        y = y_;
    }
    int height() {
        return data.size();
    }
    int width() {
        return data[0].size();
    }
    void draw_line(point ini, point end);
};
class func_1var {
};

class func_polar {
};

class func_para {
};

class axes {
};

#endif