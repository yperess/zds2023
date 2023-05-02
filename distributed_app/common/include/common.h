/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "proto/app.pb.h"

class Benchmark {
 public:
  virtual void SetUp() = 0;
  virtual void Run(app_rpc_SetValueRequest &request) = 0;
  virtual void TearDown() = 0;

  virtual void Client2ServiceLoop() = 0;
  virtual void Service2ClientLoop() = 0;
};


Benchmark* GetRpcBenchmark();
Benchmark* GetDirectBenchmark();
