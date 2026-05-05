cmake -S .. -B . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCPUSCOPE_BUILD_TESTS=ON
cmake --build .
ctest --output-on-failure