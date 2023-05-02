/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include "DemoService.hh"
#include "proto/app.pb.h"

namespace app_rpc {

void DemoService::SetValue(::pw::ConstByteSpan request, ::pw::rpc::RawUnaryResponder& responder) {
  app_rpc_SetValueResponse response = {
      .success = true,
  };
  auto bytespan = ::pw::ConstByteSpan(reinterpret_cast<std::byte*>(&response), sizeof(response));
  if (request.size_bytes() < sizeof(app_rpc_SetValueRequest)) {
    responder.Finish(bytespan, PW_STATUS_RESOURCE_EXHAUSTED);
    return;
  }

  value_ = reinterpret_cast<const app_rpc_SetValueRequest*>(request.data())->value;
  responder.Finish(bytespan);
}

void DemoService::GetValue(::pw::ConstByteSpan request, ::pw::rpc::RawUnaryResponder& responder) {
  app_rpc_GetValueResponse response = {
      .value = value_,
  };
  auto bytespan = ::pw::ConstByteSpan(reinterpret_cast<std::byte*>(&response), sizeof(response));

  responder.Finish(bytespan);
}

}  // namespace app_rpc
