#!usr/env bash

/usr/bin/cmake --no-warn-unused-cli -S./ -B./build -G Ninja

/usr/bin/cmake --build /build --config Debug --target linux --

./build/code/sorter_app/linux/linux

