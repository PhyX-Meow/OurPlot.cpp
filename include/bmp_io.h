#include <fstream>
#include <vector>

using col = unsigned char;

struct pix {
    col r;
    col g;
    col b;
};

int write(std::vector<std::vector<pix>>, const char filename[]);
int read(std::vector<std::vector<pix>>, const char filename[]);