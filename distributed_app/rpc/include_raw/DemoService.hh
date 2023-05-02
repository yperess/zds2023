/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "proto/app.pb.h"
#include "proto/app.raw_rpc.pb.h"

namespace app_rpc {

namespace pw_rpc {
using app_rpc::pw_rpc::raw::DemoService;
}  // namespace pw_rpc

class DemoService : public pw_rpc::raw::DemoService::Service<DemoService> {
 public:
  DemoService() : value_(0) {}

  void SetValue(::pw::ConstByteSpan request, ::pw::rpc::RawUnaryResponder& response);
  void GetValue(::pw::ConstByteSpan request, ::pw::rpc::RawUnaryResponder& response);

 private:
  int32_t value_;
};

}  // namespace app_rpc
