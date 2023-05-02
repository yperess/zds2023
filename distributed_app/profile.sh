#!/bin/bash
west build -p -b native_posix distributed_app/ -- -DCONFIG_GPROF=y -DCONFIG_TEST_ITERATIONS=1000000
./build/zephyr/zephyr.elf -RPC ; gprof ./build/zephyr/zephyr.elf gmon.out > analysis.rpc.txt
./build/zephyr/zephyr.elf -TRD ; gprof ./build/zephyr/zephyr.elf gmon.out > analysis.trd.txt
diff analysis.rpc.txt analysis.trd.txt > analysis.diff
gprof2dot analysis.rpc.txt | dot -Tpng -o analysis.rpc.png
