#!/bin/bash
west build -p -b native_posix samples/microservices/ -- -DCONFIG_GPROF=y -DCONFIG_TEST_ITERATIONS=1000000
./build/zephyr/zephyr.elf -RPC ; mv gmon.out ./build ; gprof ./build/zephyr/zephyr.elf ./build/gmon.out > ./build/analysis.rpc.txt
./build/zephyr/zephyr.elf -TRD ; mv gmon.out ./build ; gprof ./build/zephyr/zephyr.elf ./build/gmon.out > ./build/analysis.trd.txt
diff ./build/analysis.rpc.txt ./build/analysis.trd.txt > ./build/analysis.diff
gprof2dot ./build/analysis.rpc.txt | dot -Tpng -o ./build/analysis.rpc.png
