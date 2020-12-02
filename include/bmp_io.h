#ifndef _BMP_IO_
#define _BMP_IO_

#include "our_plot.h"
#include <fstream>
#include <sstream>

struct bmp_head {
    char id[2]{'B', 'M'};
    int sizebmp; //文件大小
    short zero1;
    short zero2;
    int offset; //文件头到图像数据的实际偏移量
};

struct dib_head {
    int length;
    int height;
    int width;
    short plane{1};
    short xiangsu;
    int zero1{0};
    int pixel_length; //说明文件大小
    int zero[4] = {0, 0, 0, 0};
};

/* BMP 读写 数组按先行后列，从左下到右上表示图像数据 */
int bmp_write(std::vector<std::vector<pix>> &data, const char filename[]);
int bmp_read(std::vector<std::vector<pix>> &data, const char filename[]);

#endif