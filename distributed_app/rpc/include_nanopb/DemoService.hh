/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "proto/app.rpc.pb.h"

namespace app_rpc {

namespace pw_rpc {
using app_rpc::pw_rpc::nanopb::DemoService;
}  // namespace pw_rpc

class DemoService : public pw_rpc::nanopb::DemoService::Service<DemoService> {
 public:
  DemoService() : value_(0) {}

  ::pw::Status SetValue(const ::app_rpc_SetValueRequest& request, ::app_rpc_SetValueResponse& response);
  ::pw::Status GetValue(const ::app_rpc_GetValueRequest& request, ::app_rpc_GetValueResponse& response);

 private:
  int32_t value_;
};

}  // namespace app_rpc
