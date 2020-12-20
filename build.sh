mkdir -p lib
clang++ -std=gnu++20 -fuse-ld=lld -shared -fpic -Wall -O2 ./src/*.cpp -o ./lib/libplot.so