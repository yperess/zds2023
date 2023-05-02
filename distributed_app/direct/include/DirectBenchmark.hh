/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <zephyr/sys/sem.h>

#include "common.h"

class DirectBenchmark : public Benchmark {
 public:
  DirectBenchmark();
  void SetUp();
  void Run(app_rpc_SetValueRequest &request);
  void TearDown();
  void Client2ServiceLoop();
  void Service2ClientLoop();
 private:
  int SetValueImpl(const app_rpc_SetValueRequest *request, app_rpc_SetValueResponse *response);
  int GetValueImpl(const app_rpc_GetValueRequest *request, app_rpc_GetValueResponse *response);
  struct k_sem sem_;
  int32_t value_;
  uint64_t server_timing_accumulator_;
  uint64_t client_timing_accumulator_;
};
