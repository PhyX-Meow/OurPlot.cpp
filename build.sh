# /usr/bin/bash
mkdir -p lib
g++ -std=gnu++14 -shared -fpic -Wall -O2 ./src/*.cpp -o ./lib/libplot.so