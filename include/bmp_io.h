#ifndef _BMP_IO_
#define _BMP_IO_

#include "our_plot.h"
#include <fstream>

struct bmp_head {
};

struct dib_head {
};

/* BMP 读写 数组按先行后列，从左下到右上表示图像数据 */
int bmp_write(std::vector<std::vector<pix>> &data, const char filename[]);
int bmp_read(std::vector<std::vector<pix>> &data, const char filename[]);

#endif