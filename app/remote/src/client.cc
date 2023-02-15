/* Copyright 2023 The ChromiumOS Authors.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <pw_hdlc/rpc_channel.h>
#include <pw_hdlc/rpc_packets.h>

namespace {
pw::hdlc::RpcChannelOutput client_to_service_channel_output(
    ClientToServiceStream().as_writer(),
    pw::hdlc::kDefaultRpcAddress,
    "HDLC client output");

pw::rpc::Channel client_channels[] = {
    pw::rpc::Channel::Create<1>(&client_to_service_channel_output),
} pw::rpc::Client rpc_client(client_channels);

}  // namespace

rpc_demo::pw_rpc::nanopb::DemoService::Client service_client(rpc_client, 1);
