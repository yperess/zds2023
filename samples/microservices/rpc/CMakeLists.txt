# Copyright (c) 2023 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)

zephyr_library_named(rpc_demo)
zephyr_library_sources(
  src/entrypoint.cc
  include/RpcBenchmark.hh
)
zephyr_library_sources_ifdef(CONFIG_RPC_TYPE_NANOPB src/DemoService_nanopb.cc)
zephyr_library_sources_ifdef(CONFIG_RPC_TYPE_RAW src/DemoService_raw.cc)

zephyr_library_include_directories(include)
zephyr_library_include_directories_ifdef(CONFIG_RPC_TYPE_NANOPB include_nanopb)
zephyr_library_include_directories_ifdef(CONFIG_RPC_TYPE_RAW    include_raw)

zephyr_library_link_libraries(kernel)

target_link_libraries(rpc_demo PRIVATE app_rpc.protos.nanopb)
target_link_libraries_ifdef(CONFIG_RPC_TYPE_NANOPB rpc_demo PRIVATE app_rpc.protos.nanopb_rpc)
target_link_libraries_ifdef(CONFIG_RPC_TYPE_RAW    rpc_demo PRIVATE app_rpc.protos.raw_rpc)
