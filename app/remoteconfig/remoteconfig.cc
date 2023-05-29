//
// Created by peress on 28/05/23.
//

#include "remoteconfig.hh"

#include <pw_rpc/nanopb/server_reader_writer.h>
#include <zephyr/logging/log.h>
#include <inttypes.h>

LOG_MODULE_REGISTER(remoteconfig);

using rpc_demo::remoteconfig::RemoteConfigService;

::pw::Status RemoteConfigService::SetValue(const ::rpc_demo_remoteconfig_SetValueRequest& request,
                                           ::rpc_demo_remoteconfig_SetValueResponse& response) {
  LOG_INF("Got new remote config '%s'=%" PRIi64, request.key, request.int_value);
  return ::pw::OkStatus();
}
