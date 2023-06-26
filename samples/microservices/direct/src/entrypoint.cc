/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include <pw_bytes/span.h>
#include <pw_hdlc/decoder.h>
#include <pw_hdlc/encoder.h>
#include <pw_hdlc/rpc_packets.h>
#include <zephyr/kernel.h>
#include <zephyr/timing/timing.h>

#include "DirectBenchmark.hh"
#include "RingReaderWriter.hh"
#include "common.h"
#include "proto/app.pb.h"

BUILD_ASSERT(sizeof(app_rpc_SetValueRequest) < CONFIG_STREAM_BUFFER_SIZE);

namespace {

// Create the buffers
std::byte client_to_service_buffer[CONFIG_STREAM_BUFFER_SIZE];
std::byte service_to_client_buffer[CONFIG_STREAM_BUFFER_SIZE];

// Create the streams
RingReaderWriter client_to_service_stream(client_to_service_buffer, "C2S");
RingReaderWriter service_to_client_stream(service_to_client_buffer, "S2C");
DirectBenchmark kDirectBenchmark;

enum DirectRequestType {
  SET_VALUE,
  GET_VALUE,
};

struct SetValueRequestData {
  app_rpc_SetValueRequest request;
  void (*success_cb)(const app_rpc_SetValueResponse*, void *);
  void (*error_cb)(int, void *);
};

struct GetValueRequestData {
  app_rpc_GetValueRequest request;
  void (*success_cb)(const app_rpc_GetValueResponse*, void *);
  void (*error_cb)(int, void *);
};

struct DirectRequest {
  enum DirectRequestType type;
  void *userdata;
  union {
    SetValueRequestData set_value;
    GetValueRequestData get_value;
  };
};

struct SetValueResponseData {
  app_rpc_SetValueResponse response;
  void (*success_cb)(const app_rpc_SetValueResponse*, void *);
  void (*error_cb)(int, void *);
};

struct GetValueResponseData {
  app_rpc_GetValueResponse response;
  void (*success_cb)(const app_rpc_GetValueResponse*, void *);
  void (*error_cb)(int, void *);
};

struct DirectResponse {
  enum DirectRequestType type;
  void *userdata;
  union {
    SetValueResponseData set_value;
    GetValueResponseData get_value;
  };
};

}  // namespace

DirectBenchmark::DirectBenchmark() : server_timing_accumulator_(0), client_timing_accumulator_(0) { k_sem_init(&sem_, 0, 1); }

void DirectBenchmark::SetUp() {}

int DirectBenchmark::SetValueImpl(const app_rpc_SetValueRequest* request, app_rpc_SetValueResponse* response) {
  value_ = request->value;
  response->success = true;
  return 0;
}

int DirectBenchmark::GetValueImpl(const app_rpc_GetValueRequest* request, app_rpc_GetValueResponse* response) {
  response->value = value_;
  return 0;
}

static void SetValueSuccessCb(const app_rpc_SetValueResponse* response, void *userdata) {
  struct k_sem *sem = reinterpret_cast<struct k_sem*>(userdata);
  k_sem_give(sem);
}

static void SetValueErrorCb(int result, void *userdata) {
  struct k_sem *sem = reinterpret_cast<struct k_sem*>(userdata);
  k_sem_give(sem);
}

static void GetValueSuccessCb(const app_rpc_GetValueResponse *response, void *userdata) {
  struct k_sem *sem = reinterpret_cast<struct k_sem*>(userdata);
  k_sem_give(sem);
}

static void GetValueErrorCb(int result, void *userdata) {
  struct k_sem *sem = reinterpret_cast<struct k_sem*>(userdata);
  k_sem_give(sem);
}

void DirectBenchmark::Run(app_rpc_SetValueRequest& request) {
  DirectRequest command = {
      .type = DirectRequestType::SET_VALUE,
      .userdata = reinterpret_cast<void*>(&sem_),
      .set_value =
          {
              .request = request,
              .success_cb = SetValueSuccessCb,
              .error_cb = SetValueErrorCb,
          },
  };
  client_to_service_stream.BeginWrite();
  while (!client_to_service_stream.as_writer().Write(reinterpret_cast<std::byte*>(&command), sizeof(command)).ok()) {
  }
  client_to_service_stream.EndWrite();
  k_sem_take(&sem_, K_FOREVER);

  memset(reinterpret_cast<void*>(&command), 0, sizeof(command));
  command.type = DirectRequestType::GET_VALUE;
  command.userdata = reinterpret_cast<void*>(&sem_),
  command.get_value.success_cb = GetValueSuccessCb;
  command.get_value.error_cb = GetValueErrorCb;
  client_to_service_stream.BeginWrite();
  while (!client_to_service_stream.as_writer().Write(reinterpret_cast<std::byte*>(&command), sizeof(command)).ok()) {
  }
  client_to_service_stream.EndWrite();
  k_sem_take(&sem_, K_FOREVER);
}

void DirectBenchmark::TearDown() {
  uint64_t server_ns = timing_cycles_to_ns(server_timing_accumulator_);
  uint64_t client_ns = timing_cycles_to_ns(client_timing_accumulator_);
  printk("THR: server processing: %010" PRIu64 " ns\n", server_ns);
  printk("THR: client processing: %010" PRIu64 " ns\n", client_ns);
  printk("THR: total processing:  %010" PRIu64 " ns\n", server_ns + client_ns);
}

void DirectBenchmark::Client2ServiceLoop() {
  DirectRequest request;
  if (!client_to_service_stream.as_reader().Read(reinterpret_cast<std::byte*>(&request), sizeof(request)).ok()) {
    return;
  }
  DirectResponse response = {
      .type = request.type,
      .userdata = request.userdata,
  };
//    timing_t start_time = timing_counter_get();
  switch (request.type) {
    case DirectRequestType::SET_VALUE:
      SetValueImpl(&request.set_value.request, &response.set_value.response);
      response.set_value.success_cb = request.set_value.success_cb;
      response.set_value.error_cb = request.set_value.error_cb;
      break;
    case DirectRequestType::GET_VALUE:
      GetValueImpl(&request.get_value.request, &response.get_value.response);
      response.get_value.success_cb = request.get_value.success_cb;
      response.get_value.error_cb = request.get_value.error_cb;
      break;
  }
//    timing_t end_time = timing_counter_get();
//    server_timing_accumulator_ += timing_cycles_get(&start_time, &end_time);
  service_to_client_stream.BeginWrite();
  while (
      !service_to_client_stream.as_writer().Write(reinterpret_cast<std::byte*>(&response), sizeof(response)).ok()) {
  }
  service_to_client_stream.EndWrite();
}

void DirectBenchmark::Service2ClientLoop() {
  DirectResponse response = {};
  if (!service_to_client_stream.as_reader().Read(reinterpret_cast<std::byte*>(&response), sizeof(response)).ok()) {
    return;
  }
//    timing_t start_time = timing_counter_get();
  switch (response.type) {
    case DirectRequestType::SET_VALUE:
      response.set_value.success_cb(&response.set_value.response, response.userdata);
      break;
    case DirectRequestType::GET_VALUE:
      response.get_value.success_cb(&response.get_value.response, response.userdata);
      break;
  }
//    timing_t end_time = timing_counter_get();
//    client_timing_accumulator_ += timing_cycles_get(&start_time, &end_time);
}

Benchmark* GetDirectBenchmark() { return reinterpret_cast<Benchmark*>(&kDirectBenchmark); }
