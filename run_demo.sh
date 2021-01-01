# /usr/bin/bash
bash ./build.sh
g++ -std=gnu++14 -Wall -O2 -L./lib -lplot demo.cpp -o demo.out
./demo.out