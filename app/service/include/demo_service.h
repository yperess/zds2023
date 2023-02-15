/* Copyright 2023 The ChromiumOS Authors.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "proto/demo.rpc.pb.h"

namespace rpc_demo {

 class DemoService : public pw_rpc::nanopb::DemoService::Service<DemoService> {
  public:
   ::pw::Status SetValue(const ::rpc_demo_SetValueRequest& request,
                         ::rpc_demo_SetValueResponse& response);

   ::pw::Status GetValue(const ::rpc_demo_GetValueRequest& request,
                         ::rpc_demo_GetValueResponse& response);

  private:
   uint32_t value;
 };
}
.venvi