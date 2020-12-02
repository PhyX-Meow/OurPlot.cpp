#ifndef _OUR_PLOT_
#define _OUR_PLOT_

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

inline double abs_d(double x) {
    return x > 0 ? x : -x;
}
inline int i_floor(double x) {
    return x >= 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}

using col = unsigned char;
struct pix {
    col b;
    col g;
    col r;
    pix() {}
    pix(col red, col green, col blue) {
        r = red;
        g = green;
        b = blue;
    }
    pix(unsigned c) {
        b = c % 256;
        c >>= 8;
        g = c % 256;
        c >>= 8;
        r = c % 256;
    }
    pix operator~() {
        return pix(~r, ~g, ~b);
    }
    pix operator<<(unsigned n) {
        pix tmp = ~(*this);
        tmp.r <<= n;
        tmp.g <<= n;
        tmp.b <<= n;
        return ~tmp;
    }
};

const pix White(0xFFFFFF);
const pix Red(0xFF0000);
const pix Green(0x00FF00);
const pix Blue(0x0000FF);
const pix Cyan(0x00FFFFF);
const pix Purple(0xFF00FF);
const pix Yellow(0xFFFF00);
const pix Black(0x000000);

struct point {
    double x;
    double y;
    point() {}
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
    double length() {
        return max - min;
    }
};
struct pix_pos {
    int clm;
    int row;
};

using img2d = std::vector<std::vector<pix>>;
using imgrow = std::vector<pix>;
struct pixrow {
    int size;
    pix *data;
    pixrow() {
        size = 0;
        data = nullptr;
    }
    pixrow(int n) {
        size = n;
        data = new pix[n];
    }
    pixrow(int n, pix color) {
        size = n;
        data = new pix[n];
        for (int i = 0; i < n; ++i) {
            data[i] = color;
        }
    }
    ~pixrow() {
        delete[] data;
    }
};

class canvas_2d {
  public:
    img2d data;
    pix &operator[](const pix_pos &pos) {
        return data[pos.row][pos.clm];
    }
    canvas_2d(int height, int width, range x_, range y_);
    int height() {
        return data.size();
    }
    int width() {
        return data[0].size();
    }
    void draw_line(point ini, point end, pix color);
    bool contains(pix_pos pos) {
        return (pos.clm < width()) && (pos.clm >= 0) && (pos.row < height()) && (pos.row >= 0);
    }
    range x;
    range y;
    pix_pos origin;
    double step_x;
    double step_y;
    point to_point(pix_pos pos);
    pix_pos to_pix(point p);
};

class func_1var {
  public:
    func_1var(double (*func_)(double), pix color_) {
        func = func_;
        set_color(color_);
    }
    func_1var(double (*func_)(double), pix color_, double precis_) {
        func = func_;
        set_color(color_);
        set_precis(precis_);
    }
    void paint_to(canvas_2d target);
    void set_color(pix color_) {
        color = color_;
    }
    pix get_color() {
        return color;
    }
    void set_precis(double precis_) {
        max_precis = precis_;
    }
    double get_precis() {
        return max_precis;
    }

  private:
    pix color;
    double (*func)(double);
    int max_precis;
};
canvas_2d &operator<<(canvas_2d &target, func_1var shape);

class func_polar {
};

class func_para {
  public: //end_cnt is just for test;
    func_para(double (*func_x_)(double), double (*func_y_)(double), double precis_, pix color_, int end_cnt_) {
        func_x = func_x_;
        func_y = func_y_;
        precis = precis_;
        color = color_;
        end_cnt = end_cnt_;
    }
    void paint_to(canvas_2d target);

  private:
    double (*func_x)(double);
    double (*func_y)(double);
    double precis;
    pix color;
    int end_cnt;
};

class axes {
};

#endif