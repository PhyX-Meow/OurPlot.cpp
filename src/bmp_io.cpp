#include "../include/bmp_io.h"

int bmp_write(img2d &data, std::string filename) {
    int height = data.size(), width = data[0].size();
    int offset = width % 4;
    int datasize = (width * 3 + offset) * height;

    bmp_head b_head;
    b_head.filesize = datasize + 0x36;
    dib_head d_head;
    d_head.width = width;
    d_head.height = height;
    d_head.datasize = datasize;

    std::ofstream file(filename, std::ios::binary);
    file.write(b_head.id, 2);
    file.write(reinterpret_cast<char *>(&b_head.filesize), 4);
    file.write(reinterpret_cast<char *>(&b_head.zero), 4);
    file.write(reinterpret_cast<char *>(&b_head.offset), 4);
    file.write(reinterpret_cast<char *>(&d_head), sizeof(d_head));

    const char pad[4]{0, 0, 0, 0};
    for (int i = 0; i < height; i++) {
        file.write(reinterpret_cast<char *>(data[i].data()), 3 * width);
        file.write(pad, offset);
    }

    file.close();
    return 0;
}

int canvas::save_as(std::string filename) {
    int failed = bmp_write(data, filename);
    if (failed)
        return failed;
    // 反馈用户写入文件正常结束
    fmt::print("Write image to {} successfully\n", filename);
    return 0;
}
int canvas::save_as(const char *filename) {
    int failed = bmp_write(data, filename);
    if (failed)
        return failed;
    // 反馈用户写入文件正常结束
    fmt::print("Write image to {} successfully\n", filename);
    return 0;
}

int canvas::attach(std::string filename) {
    if (!contains(pen))
        return 0;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        return -1;
    int height, width;
    file.seekg(18, std::ios::beg);
    file.read(reinterpret_cast<char *>(&width), 4);
    file.read(reinterpret_cast<char *>(&height), 4);
    file.seekg(0x36, std::ios::beg);
    int offset = width % 4;
    int ceiling{std::min(height, this->height() - pen.row)};
    int wall{3 * std::min(width, this->width() - pen.clm)};
    for (int i = 0; i < ceiling; i++) {
        file.read(reinterpret_cast<char *>(data[i + pen.row].data() + pen.clm), wall);
        file.seekg(offset, std::ios::cur);
    }
    file.close();
    return 0;
}