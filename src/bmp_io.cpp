#include "../include/bmp_io.h"

int bmp_write(std::vector<std::vector<pix>> &data, const char filename[]) {
    int height = data.size(), width = data[0].size();
    int setoff = width % 4;
    int filesize = (width * 3 + setoff) * height;
    bmp_head bh1 = {{'B', 'M'}, filesize + 0x36, 0, 0, 0x36};
    dib_head dh1 = {0x28, height, width, 1, 24, 0, filesize};
    std::ofstream fout(filename, std::ios::binary);
    fout.write((char *) &bh1.id[2], 4);
    fout.write((char *) &bh1.sizebmp, 4);
    fout.write((char *) &bh1.zero1, 2);
    fout.write((char *) &bh1.zero2, 2);
    fout.write((char *) &bh1.offset, 4);
    fout.write((char *) &dh1, sizeof(dh1));
    char pad[4] = {0, 0, 0, 0};
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            fout.write((char *) &data[j][i], 3);
        }
        fout.write((char *) &pad[4], setoff);
    }
    fout.close();
}
int bmp_read(std::vector<std::vector<pix>> &data, const char filename[]) {
    std::ifstream fin(filename, std::ios::binary);
    int height, width;
    fin.seekg(18, std::ios::beg);
    fin.read((char *) &height, 4);
    fin.read((char *) &width, 4);
    int setoff = width % 4;
    char pad[4] = {0, 0, 0, 0};
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            fin.read((char *) &data[j][i], 3);
        }
        fin.seekg(setoff, std::ios::beg);
    }
    fin.close();
}