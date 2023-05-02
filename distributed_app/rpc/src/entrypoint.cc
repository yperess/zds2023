/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include <pw_hdlc/rpc_channel.h>
#include <pw_hdlc/rpc_packets.h>
#include <zephyr/kernel.h>

#include "DemoService.hh"
#include "RingReaderWriter.hh"
#include "RpcBenchmark.hh"
#include "SimpleChannelOutput.hh"
#include "proto/app.pb.h"
//#include "proto/app.rpc.pb.h"

BUILD_ASSERT(sizeof(app_rpc_SetValueRequest) < CONFIG_STREAM_BUFFER_SIZE);

namespace {
constexpr uint32_t kChannelId = 1;

// Create the buffers
std::byte client_to_service_buffer[CONFIG_STREAM_BUFFER_SIZE];
std::byte service_to_client_buffer[CONFIG_STREAM_BUFFER_SIZE];

// Create the streams
RingReaderWriter client_to_service_stream(client_to_service_buffer, "C2S");
RingReaderWriter service_to_client_stream(service_to_client_buffer, "S2C");

// Create the channel outputs

#if 1
SimpleChannelOutput client_to_service_channel_output(client_to_service_stream);
SimpleChannelOutput service_to_client_channel_output(service_to_client_stream);
#else
pw::hdlc::RpcChannelOutput client_to_service_channel_output(client_to_service_stream.as_writer(),
                                                            pw::hdlc::kDefaultRpcAddress,
                                                            "HDLC client output");
pw::hdlc::RpcChannelOutput service_to_client_channel_output(service_to_client_stream.as_writer(),
                                                            pw::hdlc::kDefaultRpcAddress,
                                                            "HDLC service output");
#endif

// Configure the channels
pw::rpc::Channel client_channels[] = {
    pw::rpc::Channel::Create<kChannelId>(&client_to_service_channel_output),
};
pw::rpc::Channel service_channels[] = {
    pw::rpc::Channel::Create<kChannelId>(&service_to_client_channel_output),
};

pw::rpc::Client rpc_client(client_channels);
app_rpc::pw_rpc::DemoService::Client service_client(rpc_client, kChannelId);

static RpcBenchmark kRpcBenchmark;
}  // namespace

RpcBenchmark::RpcBenchmark()
    : server_timing_accumulator_(0),
      client_timing_accumulator_(0),
      server_decoder_accumulator_(0),
      client_decoder_accumulator_(0) {
  k_sem_init(&sem_, 0, 2);
}

void RpcBenchmark::SetUp() {}

void RpcBenchmark::Run(app_rpc_SetValueRequest& request) {
#ifdef CONFIG_RPC_TYPE_NANOPB
  auto call0 = service_client.SetValue(
      request,
      [&sem = this->sem_](const app_rpc_SetValueResponse&, pw::Status) { k_sem_give(&sem); },
      [&sem = this->sem_](pw::Status) { k_sem_give(&sem); });
#else
  auto call0 = service_client.SetValue(
      ::pw::ConstByteSpan(reinterpret_cast<std::byte*>(&request), sizeof(request)),
      [&sem = this->sem_](::pw::ConstByteSpan, pw::Status) { k_sem_give(&sem); },
      [&sem = this->sem_](pw::Status) { k_sem_give(&sem); });
#endif

  app_rpc_GetValueRequest get_value_request = app_rpc_GetValueRequest_init_default;
#ifdef CONFIG_RPC_TYPE_NANOPB
  auto call1 = service_client.GetValue(
      get_value_request,
      [&sem = this->sem_](const app_rpc_GetValueResponse&, pw::Status) { k_sem_give(&sem); },
      [&sem = this->sem_](pw::Status) { k_sem_give(&sem); });
#else
  auto call1 = service_client.GetValue(
      ::pw::ConstByteSpan(reinterpret_cast<std::byte*>(&get_value_request), sizeof(get_value_request)),
      [&sem = this->sem_](::pw::ConstByteSpan , pw::Status) { k_sem_give(&sem); },
      [&sem = this->sem_](pw::Status) { k_sem_give(&sem); });
#endif
  k_sem_take(&sem_, K_FOREVER);
  k_sem_take(&sem_, K_FOREVER);
  __ASSERT_NO_MSG(!call0.active());
  __ASSERT_NO_MSG(!call1.active());
}

void RpcBenchmark::TearDown() {
  uint64_t server_ns = timing_cycles_to_ns(server_timing_accumulator_);
  uint64_t client_ns = timing_cycles_to_ns(client_timing_accumulator_);
  uint64_t server_decoding_ns = timing_cycles_to_ns(server_decoder_accumulator_);
  uint64_t client_decoding_ns = timing_cycles_to_ns(client_decoder_accumulator_);
  printk("RPC: server processing: %010" PRIu64 " ns\n", server_ns);
  printk("RPC: client processing: %010" PRIu64 " ns\n", client_ns);
  printk("RPC: total processing:  %010" PRIu64 " ns\n", server_ns + client_ns);
  printk("RPC: server decoding:   %010" PRIu64 " ns\n", server_decoding_ns);
  printk("RPC: client decoding:   %010" PRIu64 " ns\n", client_decoding_ns);
  printk("RPC: total decoding:    %010" PRIu64 " ns\n", server_decoding_ns + client_decoding_ns);
}

void RpcBenchmark::Client2ServiceLoop() {
  std::byte decode_buffer[128];
  pw::rpc::Server server(service_channels);
  //  pw::hdlc::Decoder decoder(decode_buffer);
  SimpleDecoder decoder(decode_buffer);
  app_rpc::DemoService service;
  pw::stream::Reader& rx = client_to_service_stream.as_reader();

  server.RegisterService(service);

  while (true) {
    std::byte data[64];
    pw::Result<pw::span<std::byte>> read_result = rx.Read(data, ARRAY_SIZE(data));
    if (!read_result.ok()) {
      continue;
    }
    for (size_t idx = 0; idx < read_result.value().size(); ++idx) {
      //      timing_t start_time = timing_counter_get();
      auto result = decoder.Process(data[idx]);
      //      timing_t end_time = timing_counter_get();
      //      server_decoder_accumulator_ += timing_cycles_get(&start_time, &end_time);
      if (!result.ok()) {
        continue;
      }

      auto& frame = result.value();
      if (frame.address() == pw::hdlc::kDefaultRpcAddress) {
//        timing_t start_time = timing_counter_get();
        server.ProcessPacket(frame.data());
//        timing_t end_time = timing_counter_get();
//        server_timing_accumulator_ += timing_cycles_get(&start_time, &end_time);
        decoder.Clear();
      }
    }
  }
}

void RpcBenchmark::Service2ClientLoop() {
  std::byte decode_buffer[128];
  //  pw::hdlc::Decoder decoder(decode_buffer);
  SimpleDecoder decoder(decode_buffer);
  pw::stream::Reader& rx = service_to_client_stream.as_reader();
  while (true) {
    std::byte data[64];
    pw::Result<pw::span<std::byte>> read_result = rx.Read(data, ARRAY_SIZE(data));
    if (!read_result.ok()) {
      continue;
    }

    for (size_t idx = 0; idx < read_result.value().size(); ++idx) {
      //      timing_t start_time = timing_counter_get();
      auto result = decoder.Process(data[idx]);
      //      timing_t end_time = timing_counter_get();
      //      client_decoder_accumulator_ += timing_cycles_get(&start_time, &end_time);
      if (!result.ok()) {
        continue;
      }
      auto& frame = result.value();
      if (frame.address() == pw::hdlc::kDefaultRpcAddress) {
//        timing_t start_time = timing_counter_get();
        rpc_client.ProcessPacket(frame.data());
//        timing_t end_time = timing_counter_get();
//        client_timing_accumulator_ += timing_cycles_get(&start_time, &end_time);
        decoder.Clear();
      }
    }
  }
}

Benchmark* GetRpcBenchmark() { return reinterpret_cast<Benchmark*>(&kRpcBenchmark); }
