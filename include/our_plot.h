#ifndef _OUR_PLOT_
#define _OUR_PLOT_

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

inline double abs_d(double x) {
    return x > 0 ? x : -x;
}

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

const pix Black(0, 0, 0);
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
    double length() {
        return max - min;
    }
};
struct pix_pos {
    int row;
    int clm;
};

using img2d = std::vector<std::vector<pix>>;
using imgrow = std::vector<pix>;

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

class shape_2d {
  public:
    void paint_to(canvas_2d target);
    void set_color(pix color_) {
        color = color_;
    }
    pix get_color() {
        return color;
    }

  private:
    pix color;
};
canvas_2d &operator<<(canvas_2d target, shape_2d shape);

class func_1var : public shape_2d {
  public:
    func_1var(double (*func_)(double), double precis_, pix color_) {
        func = func_;
        precis = precis_;
        set_color(color_);
    }
    void paint_to(canvas_2d target);

  private:
    double (*func)(double);
    double precis;
};

class func_polar {
};

class func_para {
  public: //end_cnt is just for test;
    func_para(double (*func_x_)(double), double (*func_y_)(double), double precis_, pix color_, canvas_2d target_canvas, int end_cnt_) {
        func_x = func_x_;
        func_y = func_y_;
        precis = precis_;
        color = color_;
        draw(target_canvas);
        end_cnt = end_cnt_;
    }
    void draw(canvas_2d target_canvas);

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