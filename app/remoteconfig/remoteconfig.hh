#pragma once

#include "proto/demo.rpc.pb.h"

namespace rpc_demo::remoteconfig {

class RemoteConfigService final : public pw_rpc::nanopb::RemoteConfig::Service<RemoteConfigService> {
 public:
  RemoteConfigService() {};

  ::pw::Status SetValue(const ::rpc_demo_remoteconfig_SetValueRequest& request,
                        ::rpc_demo_remoteconfig_SetValueResponse& response);
};

}  // namespace rpc_demo::remoteconfig
