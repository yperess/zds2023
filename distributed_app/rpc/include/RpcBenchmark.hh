/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <zephyr/sys/sem.h>
#include <zephyr/timing/timing.h>

#include "common.h"

class RpcBenchmark : public Benchmark {
 public:
  RpcBenchmark();
  void SetUp();
  void Run(app_rpc_SetValueRequest &request);
  void TearDown();
  void Client2ServiceLoop();
  void Service2ClientLoop();
 private:
  uint64_t server_timing_accumulator_;
  uint64_t client_timing_accumulator_;
  uint64_t server_decoder_accumulator_;
  uint64_t client_decoder_accumulator_;
  struct k_sem sem_;
};
