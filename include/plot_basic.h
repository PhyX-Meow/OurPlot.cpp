#ifndef _PLOT_BASIC_
#define _PLOT_BASIC_

#include <vector>

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

using pixrow = std::vector<pix>;
using img2d = std::vector<std::vector<pix>>;

#endif