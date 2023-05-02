# Copyright (c) 2023 Google LLC
# SPDX-License-Identifier: Apache-2.0

pw_proto_library(app_rpc.protos
  SOURCES
    proto/app.proto
)

add_library(app_common
  INTERFACE
    include/common.h
    include/RingReaderWriter.hh
)
target_link_libraries(app_common INTERFACE app_rpc.protos.nanopb)
target_include_directories(app_common INTERFACE include)
zephyr_link_libraries(app_common)
