# /usr/bin/bash
bash ./build.sh
clang++ -std=gnu++20 -stdlib=libc++ -fuse-ld=lld -Wall -O2 -L./lib -lplot demo.cpp -o demo.out
./demo.out