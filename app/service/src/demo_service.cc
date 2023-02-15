/* Copyright 2023 The ChromiumOS Authors.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "demo_service.h"

namespace rpc_demo {
::pw::Status DemoService::SetValue(const ::rpc_demo_SetValueRequest& request,
                                   ::rpc_demo_SetValueResponse& response) {
  value = request.value;
  response.status = ::rpc_demo_Status_SUCCESS;
  return ::pw::OkStatus();
}

::pw::Status DemoService::GetValue(const ::rpc_demo_GetValueRequest& request,
                                   ::rpc_demo_GetValueResponse& response) {
  response.value = value;
  return ::pw::OkStatus();
}

}
