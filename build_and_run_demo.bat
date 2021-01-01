cd /d %~dp0
g++ -std=gnu++14 -O2 ./src/bmp_io.cpp ./src/plot_2d.cpp ./src/plot_3d.cpp ./src/toy_3d.cpp ./demo.cpp -o demo.exe
demo.exe