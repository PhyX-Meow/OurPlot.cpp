bash ./build.sh
clang++ -std=gnu++20 -fuse-ld=lld -Wall -O2 -L./lib -lplot -lfmt demo.cpp -o demo.out
./demo.out