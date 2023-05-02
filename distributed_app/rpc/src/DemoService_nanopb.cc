/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include "DemoService.hh"

namespace app_rpc {

::pw::Status DemoService::SetValue(const ::app_rpc_SetValueRequest& request,
                                   ::app_rpc_SetValueResponse& response) {
  value_ = request.value;
  response.success = true;
  return ::pw::OkStatus();
}

::pw::Status DemoService::GetValue(const ::app_rpc_GetValueRequest& request,
                                   ::app_rpc_GetValueResponse& response) {
  response.value = value_;
  return ::pw::OkStatus();
}

}  // namespace app_rpc
