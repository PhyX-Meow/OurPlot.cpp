#ifndef _PLOT_BASIC_
#define _PLOT_BASIC_

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/format.h"
#include <cmath>
#include <string>
#include <vector>

const double pi = M_PI;

// i_floor : double -> int
inline int i_floor(double x) {
    x += 1e-5;
    return x >= 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}

struct range {
    double min;
    double max;
    double length() {
        return max - min;
    }
};

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
    pix min(pix color) {
        pix tmp = (*this);
        tmp.r = tmp.r < color.r ? tmp.r : color.r;
        tmp.g = tmp.g < color.g ? tmp.g : color.g;
        tmp.b = tmp.b < color.b ? tmp.b : color.b;
        return tmp;
    }
    pix operator*(double k) {
        pix tmp = ~(*this);
        tmp.r = static_cast<char>(tmp.r * k);
        tmp.g = static_cast<char>(tmp.g * k);
        tmp.b = static_cast<char>(tmp.b * k);
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

using pixrow = std::vector<pix>;
using img2d = std::vector<std::vector<pix>>;

struct pix_pos {
    int clm;
    int row;
    pix_pos add(const pix_pos &adder) {
        return {clm + adder.clm, row + adder.row};
    }
    pix_pos add(int n, int m) {
        return {clm + n, row + m};
    }
    void operator+=(const pix_pos &adder) {
        clm += adder.clm;
        row += adder.row;
    }
};
struct vector_2d {
    double x;
    double y;
    double operator*(vector_2d &a) {
        return a.x * (*this).x + a.y * (*this).y;
    }
    vector_2d operator*(double &c) {
        return {(*this).x * c, (*this).y * c};
    }
    vector_2d operator-(vector_2d &a) {
        return {(*this).x - a.x, (*this).y - a.y};
    }
    vector_2d proj_to(vector_2d &a, double min, double max) {
        double c{((*this) * a / (a * a))};
        c = c > min ? c : min;
        c = c < max ? c : max;
        return a * c;
    }
};
struct float_pos {
    double x;
    double y;
};

enum plot_style {
    thin,
    medium,
    thick
};

class canvas {
  public:
    img2d data;
    pix &operator[](const pix_pos &pos) {
        return data[pos.row][pos.clm];
    }

    int height() { return data.size(); }
    int width() { return data[0].size(); }

    bool contains(pix_pos pos) {
        return (pos.clm < width()) && (pos.clm >= 0) && (pos.row < height()) && (pos.row >= 0);
    }
    pix_pos to_pix(float_pos p) {
        return {i_floor(p.x) + origin.clm, i_floor(p.y) + origin.row};
    }
    float_pos to_affine(pix_pos pos) {
        return {pos.clm - origin.clm + 0.5, pos.row - origin.row + 0.5};
    }

    void draw_line(float_pos ini, float_pos end, pix color, plot_style style = thin);
    int save_as(std::string filename);
    int attach(pix_pos base, std::string filename);

    pix_pos origin;
};

#endif