/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_ARCH_POSIX
#include "posix_board_if.h"
#endif

#include <zephyr/kernel.h>
#include <zephyr/timing/timing.h>

#include "common.h"

#define STACK_SIZE 2048

static void service_reader_thread(void* p0, void*, void*) {
  Benchmark* benchmark = static_cast<Benchmark*>(p0);
  while (true) {
    benchmark->Client2ServiceLoop();
  }
}

static void client_reader_thread(void* p0, void*, void*) {
  Benchmark* benchmark = static_cast<Benchmark*>(p0);
  while (true) {
    benchmark->Service2ClientLoop();
  }
}

K_KERNEL_STACK_DEFINE(service_reader_stack, STACK_SIZE);
K_KERNEL_STACK_DEFINE(client_reader_stack, STACK_SIZE);

static struct k_thread service_reader;
static struct k_thread client_reader;

static uint64_t RunTests(Benchmark* benchmark) {
  if (benchmark == nullptr) {
    return 0;
  }
  benchmark->SetUp();
  k_tid_t service_reader_tid = k_thread_create(&service_reader,
                                               service_reader_stack,
                                               K_KERNEL_STACK_SIZEOF(service_reader_stack),
                                               service_reader_thread,
                                               benchmark,
                                               NULL,
                                               NULL,
                                               0,
                                               0,
                                               K_NO_WAIT);
  k_tid_t client_reader_tid = k_thread_create(&client_reader,
                                              client_reader_stack,
                                              K_KERNEL_STACK_SIZEOF(client_reader_stack),
                                              client_reader_thread,
                                              benchmark,
                                              NULL,
                                              NULL,
                                              0,
                                              0,
                                              K_NO_WAIT);
  int32_t count = 0;
  timing_t start_time = timing_counter_get(), end_time;
  do {
    app_rpc_SetValueRequest request = {
        .value = count++,
    };
    benchmark->Run(request);
  } while (count < CONFIG_TEST_ITERATIONS);
  end_time = timing_counter_get();
  k_thread_abort(service_reader_tid);
  k_thread_abort(client_reader_tid);
  benchmark->TearDown();
  return timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
}

static bool run_rpc_test;
static bool run_threaded_test;
#ifdef CONFIG_ARCH_POSIX
#include <cmdline.h>
#include <soc.h>
static void add_profiling_filter_options() {
  static char cmd_rpc[] = "RPC";
  static char cmd_rpc_help[] = "Enable the RPC benchmark";
  static char cmd_threaded[] = "TRD";
  static char cmd_threaded_help[] = "Enable the THREADED benchmark";
  static struct args_struct_t profiling_filter_s[] = {
      {
          .manual = false,
          .is_mandatory = false,
          .is_switch = true,
          .option = cmd_rpc,
          .name = nullptr,
          .type = 'b',
          .dest = &run_rpc_test,
          .call_when_found = nullptr,
          .descript = cmd_rpc_help,
      },
      {
          .manual = false,
          .is_mandatory = false,
          .is_switch = true,
          .option = cmd_threaded,
          .name = nullptr,
          .type = 'b',
          .dest = &run_threaded_test,
          .call_when_found = nullptr,
          .descript = cmd_threaded_help,
      },
  };
  native_add_command_line_opts(profiling_filter_s);
}
NATIVE_TASK(add_profiling_filter_options, PRE_BOOT_1, 10);
#endif // CONFIG_ARCH_POSIX

int main() {
  bool run_all = !run_threaded_test && !run_rpc_test;
  timing_init();
  timing_start();
  uint64_t direct_benchmark_ns = 0;
  uint64_t rpc_benchmark_ns = 0;
  if (run_all || run_rpc_test) {
    printk("Running RPC benchmarks...\n");
    rpc_benchmark_ns = RunTests(GetRpcBenchmark());
    printk("RPC execution time   : %010" PRIu64 " ns\n", rpc_benchmark_ns);
  }
  if (run_all || run_threaded_test) {
    printk("Running direct benchmarks...\n");
    direct_benchmark_ns = RunTests(GetDirectBenchmark());
    printk("Thread execution time: %010" PRIu64 " ns\n", direct_benchmark_ns);
  }

//  uint64_t percent = rpc_benchmark_ns * UINT64_C(100000000) / direct_benchmark_ns;
//  printk("RPC cost             : %02llu.%06llu%%\n", percent / 1000000, percent % 1000000);

#ifdef CONFIG_ARCH_POSIX
  posix_exit(0);
#endif
  return 0;
}
