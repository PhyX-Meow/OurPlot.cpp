#ifndef _BMP_IO_
#define _BMP_IO_

#include "plot_basic.h"
#include <fstream>

struct bmp_head {
    char id[2]{'B', 'M'};
    int filesize; // 文件大小
    short zero[2]{0, 0};
    int offset{0x36}; // 文件头到图像数据的实际偏移量
};

struct dib_head {
    int length{0x28};
    int width;
    int height;
    short plane{1};
    short pixiel{24};
    int zero1{0};
    int datasize; // 说明文件大小
    int zero[4]{0, 0, 0, 0};
};

int bmp_write(img2d &data, std::string filename);

#endif