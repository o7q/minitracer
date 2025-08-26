mkdir build
cmake -S . -B build
cmake --build build -j8
cp ./build/minitracer ./minitracer
